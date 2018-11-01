const tester = require('gitbook-tester');
const test = require('tape');

const pkg = require('./package.json');

test.skip('Creates a select dropdown with versions', (t) => {

  t.plan(1);

  tester.builder()
    .withContent('```js\nfunction() { return true };\n```')
    .withLocalPlugin(__dirname)
    .withBookJson({
      gitbook: pkg.engines.gitbook,
      plugins: ['versions-select']
    })
    .create()
    .then(function(result) {
      var expected = 'versions';
      // TODO: Use result [1]
      t.equal(result[6].content.indexOf(expected) !== -1, true);
    })
    .done();

});
