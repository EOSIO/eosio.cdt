COLORS = {
    "black": 0,
    "red": 1,
    "green": 2,
    "yellow": 3,
    "blue": 4,
    "magenta": 5,
    "cyan": 6,
    "white": 7,
}

STYLES = {"reset": 0, "bold": 1, "italic": 3, "underline": 4}


class Printer:
    verbose = False

    @staticmethod
    def print(*text, verbose=False, newline=True):
        end = "\n" if newline else ""

        if not verbose:
            print(*text, end=end)

        if verbose and Printer.verbose:
            print(*text, end=end)

    # @staticmethod
    # def bold(*text, **kwargs):
    # return decorate(*text, style="bold", **kwargs)

    @staticmethod
    def red(*text, verbose=False, newline=True):
        if not verbose:
            print_decorated(*text, fcolor="red", newline=newline)

        if verbose and Printer.verbose:
            print_decorated(*text, fcolor="red", newline=newline)

    @staticmethod
    def green(*text, verbose=False, newline=True):
        if not verbose:
            print_decorated(*text, fcolor="green", newline=newline)

        if verbose and Printer.verbose:
            print_decorated(*text, fcolor="green", newline=newline)

    # @staticmethod
    # def yellow(*text, **kwargs):
    # return decorate(*text, fcolor="yellow", **kwargs)

    # @staticmethod
    # def blue(*text, **kwargs):
    # return decorate(*text, fcolor="blue", **kwargs)

    # @staticmethod
    # def magenta(*text, **kwargs):
    # return decorate(*text, fcolor="magenta", **kwargs)

    # @staticmethod
    # def cyan(*text, **kwargs):
    # return decorate(*text, fcolor="cyan", **kwargs)

    # @staticmethod
    # def white(*text, **kwargs):
    # return decorate(*text, fcolor="white", **kwargs)


def print_decorated(*text, style=None, fcolor=None, bcolor=None, newline=True):
    text_format = parse(style, fcolor, bcolor)
    end = "\n" if newline else ""
    print(f"\033[{text_format}m", end="")
    print(*text, end="")
    print("\033[0m", end=end)


def parse(style="", fcolor="", bcolor=""):
    attr = [] if not style else [STYLES[style]]

    if fcolor:
        attr.append(30 + COLORS[fcolor])

    if bcolor:
        attr.append(40 + COLORS[bcolor])

    return ";".join([str(x) for x in attr])
