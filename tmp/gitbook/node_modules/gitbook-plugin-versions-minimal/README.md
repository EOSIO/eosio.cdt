## gitbook-plugin-versions-select

[![NPM version][npm-image]][npm-url]
[![NPM downloads][npm-downloads-image]][npm-url]
[![License][license-image]][license-url]
[![Code Style][code-style-image]][code-style-url]

> This is a modified version of [Versions](https://plugins.gitbook.com/plugin/versions)

Display a `<select>` with other versions of your gitbook that you have specified in your `book.json` file.

### Usage with gitbook.com

When your book is hosted on [GitBook.com](https://www.gitbook.com), the plugin can access a listing of versions using the [GitBook API](http://developer.gitbook.com/books/versions/).

Put this in your book.json:

```js
{
  "plugins": [ "versions-select" ],
  "pluginsConfig": {
    "versions": {
      "gitbookConfigURL": "https://raw.githubusercontent.com/prescottprue/react-redux-firebase/v2.0.0/book.json",
      "options": [
        {
          "value": "http://react-redux-firebase.com/history/v1.4.0/",
          "text": "Version v1.4.0"
        },
        {
          "value": "http://react-redux-firebase.com/history/v1.4.0/",
          "text": "Version v2.0.0"
        }
      ]
    }
  }
}
```

A `<select>` element will be created with the given `options` and placed at the top of the book summary. When the user selects one of the options, they are taken to that URL.

The `gitbookConfigURL` variable is a publicly accessible URL to your `book.json`. If this is present, the plugin will fetch the latest config when the page loads, so even older versions of your book will have updated `options`.

## FAQ

1. How is this different than [`gitbook-plugin-versions`](https://plugins.gitbook.com/plugin/versions)

  The active version selected in the `<select>` is based location.

1. How is this different than [`gitbook-plugin-feathers-versions`](https://github.com/feathersjs/gitbook-plugin-feathers-versions)?

  Versions on the same domain under different folders are supported

## Credits

Original work by @mjackson.

## License

Apache 2


[npm-image]: https://img.shields.io/npm/v/gitbook-plugin-versions-select.svg?style=flat-square
[npm-url]: https://npmjs.org/package/gitbook-plugin-versions-select
[npm-downloads-image]: https://img.shields.io/npm/dm/gitbook-plugin-versions-select.svg?style=flat-square
[license-image]: https://img.shields.io/npm/l/gitbook-plugin-versions-select.svg?style=flat-square
[license-url]: https://github.com/prescottprue/gitbook-plugin-versions-select/blob/master/LICENSE
[code-style-image]: https://img.shields.io/badge/code%20style-standard-brightgreen.svg?style=flat-square
[code-style-url]: http://standardjs.com/
