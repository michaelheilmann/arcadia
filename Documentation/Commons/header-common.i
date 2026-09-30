<!DOCTYPE html>
<html class='my-theme-1' lang='en'>
<head>

  <meta charset='utf-8'>
  <meta name='viewport' content='width=device-width, initial-scale=1'>

  <link rel='icon' type='image/svg+xml' href='@{siteAddress}/assets/favicon/512x512.svg'>
  <link rel="icon" type="image/png" sizes="64x64" href="@{siteAddress}/assets/favicon/64x64.png">
  <link rel="icon" type="image/png" sizes="32x32" href="@{siteAddress}/assets/favicon/32x32.png">

  <!-- Crap specific to Safari. SVG must be single color with transparent background. -->
  <link rel='mask-icon' href='@{siteAddress}/assets/favicon/safari-mask-icon-512x512.svg' color = '#000000'>

  <!-- Crap specific to iOS. PNG must be 180 x 180 pixels. color attribute is obsolete. -->
  <link rel='apple-touch-icon' href='@{siteAddress}/assets/favicon/apple-touch-icon-180x180.png'>

  <script>
  MathJax = {
    startup: {
      ready() {
        MathJax.startup.defaultReady();
        const {Token} = MathJax._.input.tex.Token;
        const {MapHandler} = MathJax._.input.tex.MapHandler;
        const delimiter = MapHandler.getMap('delimiter');
        delimiter.add('\\llbracket', new Token('\\llbracket', '\u27E6'));
        delimiter.add('\\rrbracket', new Token('\\rrbracket', '\u27E7'));
      }
    }
  }
  </script>

  <!-- @@todo Do not use external CDNs. Do use self-hosting. -->
  <script defer id="MathJax-script" async src="https://cdn.jsdelivr.net/npm/mathjax@@4/tex-mml-chtml.js"></script>

  <!-- Cascading Style Sheets. -->
  <link rel='stylesheet' href='@{siteAddress}/assets/reset.css?v=5'>
  <link rel='stylesheet' href='@{siteAddress}/assets/index.css?v=7'>
