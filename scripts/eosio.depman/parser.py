import dependency
import util
from logger import err, warn, log
from lark import Lark

class parser:
    parser = Lark(r"""
            start: dependencies (urls|packages|commands|groups|relative_prefix|repo)*
            dependencies: DEP_TAG (dep_rule_pair)*

            dep_rule_pair: STR ":" dep_tuple
            dep_tuple: "[" (GE? VERSION) "," DEP_TYPE "," BUILD_TYPE ("," system_type ":" STR)* "]"

            version: "<" GE? VERSION ">"
            system_type: (STR version?) | ALL 
            command_type: (PREBUILD | BUILD | INSTALL)
            constraint: "(" STRING ")"
            
            urls: URL_TAG (url_rule_pair)*
            url_rule_pair: STR ":" "[" url_rule ("," url_rule)* "]"
            url_rule: URL_TYPE ":" system_type (":" constraint)* ":" URL

            packages: PACKAGE_TAG (package_rule_pair)*
            package_rule_pair: STR ":" "[" package_rule ("," package_rule)* "]"
            package_rule: (system_type (":" constraint)* ":" PACKAGE_NAME)
            
            commands: COMMANDS_TAG (command_rule_pair)*
            command_rule_pair: STR ":" "[" command_rule ("," command_rule)* "]"
            command_rule: command_type ":" system_type (":" constraint)* ":" ESCAPED_STRING

            groups: GROUPS_TAG (group)*
            group: GROUP_TAG (group_item)*
            group_item: STR ":" system_type (":" constraint)*
            
            relative_prefix: RELATIVE_PREFIX_TAG (relative_prefix_pair)*
            relative_prefix_pair: STR ":" "[" relative_prefix_rule ("," relative_prefix_rule)* "]" 
            relative_prefix_rule: system_type (":" constraint)* ":" ESCAPED_STRING

            repo: REPO_TAG (repo_pair)*
            repo_pair: system_type ":" "[" repo_rule ("," repo_rule)* "]"
            repo_rule: ESCAPED_STRING

            COMMENT: /#[^\n]*/
            STR: /[a-zA-Z_][\w\.-]*/
            SSTR: /[a-zA-Z_-]+/
            GE: />=/
            STRING : /[ubf]?r?("(?!"").*?(?<!\\)(\\\\)*?"|'(?!'').*?(?<!\\)(\\\\)*?')/i
            DEP_TAG: /\[[ \t\n\r]*dependencies[ \t\n\r]*\]/
            URL_TAG: /\[[ \t\n\r]*urls[ \t\n\r]*\]/
            PACKAGE_TAG: /\[[ \t\n\r]*packages[ \t\n\r]*\]/
            COMMANDS_TAG: /\[[ \t\n\r]*commands[ \t\n\r]*\]/
            RELATIVE_PREFIX_TAG: /\[[ \t\n\r]*relative\-prefix[ \t\n\r]*\]/
            REPO_TAG: /\[[ \t\n\r]*repo[ \t\n\r]*\]/
            GROUPS_TAG: /\[[ \t\n\r]*groups[ \t\n\r]*\]/
            GROUP_TAG: (/\[[ \t\n\r]*/)(/optional::/)? STR (/[ \t\n\r]*\]/)
            VERSION: /([0-9]+\.[0-9]+)|[0-9]+|any/
            PACKAGE_NAME: SSTR (/-|@/)? VERSION? (/[\-0-9]/)* SSTR* (/-dev/)?
            PREBUILD: /pre-build/
            BUILD: /build/
            INSTALL: /install/
            DEP_TYPE: /(exe|lib)/
            URL_TYPE: /(bin|source)/
            URL: /http[s]?://(?:[a-zA-Z]|[0-9]|[$-_@.&+]|[!*\(\),]|(?:%[0-9a-fA-F][0-9a-fA-F]))+(bz2|tgz|gz|xz|zip|tar)/
            ALL: /all/
            BUILD_TYPE: /none/ | STR
            ANY: /..*/

            %import common.ESCAPED_STRING
            %import common.SIGNED_NUMBER
            %import common.WS
            %ignore WS
            %ignore COMMENT
            """, start="start")

    def parse(self, fname):
        dep_file = open(fname, "r")
        dep_str = ""
        for line in dep_file:
            dep_str += line
        
        tagged_dependencies = dict()
        dependencies = dict()
        repos = list()
        obj = self.parser.parse( dep_str )

        os_name, dist, ver = util.get_os()

        for child in obj.children:
            if child.data == "dependencies":
                for cc in child.children[1:]:
                    dep_name = cc.children[0]
                    is_strict = False
                    if len(cc.children[1].children) > 3:
                        ge, version, dep_type, build_type = cc.children[1].children
                        is_strict = (str(ge) != ">=")
                    else:
                        version, dep_type, build_type = cc.children[1].children

                    vers = None
                    if version == "any":
                        vers = dependency.version(-1, -1)
                    else:
                        vers = dependency.version(int(version.split(".")[0]), int(version.split(".")[1]))
                    dependencies[str(dep_name)] = dependency.dependency( str(dep_name), vers, str(dep_type), str(build_type) )
                    dependencies[str(dep_name)].strict = is_strict
            elif child.data == "urls":
                for cc in child.children[1:]:
                    for ccc in cc.children[1:]:
                        should_add = True
                        if len(ccc.children) == 3:
                            url_type, system_type, url = ccc.children 
                        else:
                            url_type, system_type, constraint, url = ccc.children 
                            cons = constraint.children[0]
                            cons = cons.lstrip('\'').rstrip('\'')
                            should_add = eval(cons)

                        os_str = system_type.children[0]
                        if (len(system_type.children) > 1):
                            os_str += "<"+system_type.children[1].children[0]+">"
                        if should_add and \
                           os_str == "all" or \
                           os_str == dist  or \
                           os_str == dist+"<"+ver+">":
                            if not cc.children[0] in dependencies:    
                                err.log("Dependency "+cc.children[0]+" not defined in dependencies section")
                            if url_type == "bin":
                                if not dependencies[cc.children[0]].bin_url:
                                    dependencies[cc.children[0]].bin_url = str(url)
                            else:
                                if not dependencies[cc.children[0]].source_url:
                                    dependencies[cc.children[0]].source_url = str(url)

            elif child.data == "relative-prefix":
                print("REL")
                print(child.children[1:])

            elif child.data == "repo":
                for cc in child.children[1:]:
                    sys_type = child.children[1].children[0].children[0]
                    if len(child.children[1].children[0].children) > 1:
                        sys_type += "<"+child.children[1].children[0].children[1].children[0]+">"

                    if should_add and \
                       sys_type == "all" or \
                       sys_type == dist  or \
                       sys_type == dist+"<"+ver+">":
                        for ccc in cc.children[1:]:
                            repos.append(ccc.children[0].lstrip('"').rstrip('"'))

            elif child.data == "packages":
                for cc in child.children[1:]:
                    for ccc in cc.children[1:]:
                        should_add = True
                        if len(ccc.children) == 2:
                            system_type, package_name = ccc.children 
                        else:
                            system_type, constraint, package_name = ccc.children 
                            cons = constraint.children[0]
                            cons = cons.lstrip('\'').rstrip('\'')
                            should_add = eval(cons)
                        os_str = system_type.children[0]
                        if (len(system_type.children) > 1):
                            os_str += "<"+system_type.children[1].children[0]+">"
                        if should_add and \
                           os_str == "all" or \
                           os_str == dist  or \
                           os_str == dist+"<"+ver+">":
                            if not cc.children[0] in dependencies:    
                                err.log("Dependency "+cc.children[0]+" not defined in dependencies section")
                            if dependencies[cc.children[0]].package_name == "***":
                                dependencies[cc.children[0]].package_name = (package_name)

            elif child.data == "commands":
                for cc in child.children[1:]:
                    for ccc in cc.children[1:]:
                        should_add = True
                        if len(ccc.children) == 3:
                            command_type, system_type, cmd = ccc.children 
                        else:
                            command_type, system_type, constraint, cmd = ccc.children 
                            cons = constraint.children[0]
                            cons = cons.lstrip('\'').rstrip('\'')
                            should_add = eval(cons)
                        os_str = system_type.children[0]
                        if (len(system_type.children) > 1):
                            os_str += "<"+system_type.children[1].children[0]+">"
                        if should_add and \
                           os_str == "all" or \
                           os_str == dist  or \
                           os_str == dist+"<"+ver+">":
                            if not cc.children[0] in dependencies:    
                                err.log("Dependency "+cc.children[0]+" not defined in dependencies section")
                            if command_type.children[0] == "pre-build":
                                if not dependencies[cc.children[0]].pre_build_cmds:
                                    dependencies[cc.children[0]].pre_build_cmds = (cmd.lstrip("\"").rstrip("\""))
                            elif command_type.children[0] == "build":
                                if not dependencies[cc.children[0]].build_cmds:
                                    dependencies[cc.children[0]].build_cmds = (cmd.lstrip("\"").rstrip("\""))
                            else:
                                if not dependencies[cc.children[0]].install_cmds:
                                    dependencies[cc.children[0]].install_cmds = (cmd.lstrip("\"").rstrip("\""))

            elif child.data == "groups":
                for cc in child.children[1:]:
                    group = cc.children[0].lstrip('[').rstrip(']')
                    for ccc in cc.children[1:]:
                        os_str = ccc.children[1].children[0]
                        if (len(ccc.children[1].children) > 1):
                            os_str += "<"+ccc.children[1].children[1].children[0]+">"
                        if should_add and \
                           os_str == "all" or \
                           os_str == dist  or \
                           os_str == dist+"<"+ver+">":
                           if not group in tagged_dependencies:
                               tagged_dependencies[group] = list()
                           tagged_dependencies[group].append(str(ccc.children[0]))

        return [dependencies, tagged_dependencies, repos] 
