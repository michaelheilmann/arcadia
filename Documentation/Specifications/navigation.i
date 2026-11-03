<!-- Progressive enhancement: without JavaScript, native contents stay expanded. -->
<script>
(() => {
  const compact = window.matchMedia('(max-width: 70rem)');
  const contents = document.querySelectorAll('details.specification-contents');
  const update = () => {
    for (const element of contents) element.open = !compact.matches;
  };
  update();
  compact.addEventListener('change', update);
})();
</script>
