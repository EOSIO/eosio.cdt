require(['gitbook', 'jQuery'], function (gitbook, $) {
  var versions = [];
  var current;
  var pluginConfig = {};

  // Update the select with a list of versions
  function updateVersions(_versions) {
    versions = _versions || versions;
    current = $('.versions-minimal select').val() || current;

    // Cleanup existing selector
    $('.versions-minimal').remove();

    if (versions.length === 0) return;

    var $li = $('<li>', {
      'class': 'versions-minimal',
      'html': '<div><select></select></div>'
    });
    var $select = $li.find('select');

    $.each(versions, function(i, version) {
      var $option = $('<option>', {
        'selected': window.location.href.indexOf(version.value) !== -1,
        'value': version.value,
        'text': version.value
      });

      $option.appendTo($select);
    });

    $select.change(function() {
      var filtered = $.grep(versions, function(v) {
        return v.value === $select.val();
      });
            // Get actual version Object from array
      var version = filtered[0];

      var filePath = window.location.href.replace(gitbook.state.bookRoot, '');
      window.location.href = version.includeFilepath ? (version.value + filePath) : version.value;
    });

    $li.prependTo('.book-summary ul.summary');
  }

    // Fetch version from book.json (legacy plugin)
  function fetchVersions(gitbookConfigURL) {
    $.getJSON(gitbookConfigURL, function (bookConfig) {
      updateVersions(bookConfig);
    });
  }

  gitbook.events.bind('start', function (e, config) {
    pluginConfig = config.versions || {};
    fetchVersions(pluginConfig.gitbookConfigURL);
  });

  gitbook.events.bind('page.change', function () {
    updateVersions();
  });
});
