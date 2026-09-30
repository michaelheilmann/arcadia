/* Copyright (c) 2024-2025 Michael Heilmann. All rights reserved. */
div.topic div p a {
  font-size: var(--default-font-size);
  text-decoration: none;
}

body.my-index-page-1 div.topic div.header {
  font-size: var(--h6-font-size);
}

body.my-index-page-1 div.topic div.body {
  font-size: var(--default-font-size);

  padding-top: 1rem;
  margin-top: 1rem;
  border-top-style: solid;
}

body.my-index-page-1 div.topic:not(:first-child),
body.my-subindex-page-1 div.topic:not(:first-child) {
  border-left-style: solid;
  border-left-width: 1px;
}

body.my-index-page-1 div.topic {
  display: flex;
  flex-direction: column;
  background-color: rgb(250,250,250);
  padding-left: 16px;
  padding-right: 16px;
  margin-top: 16px;
  margin-bottom: 16px;
  width: calc(100%/3);
}

body.my-index-page-1 div.topic {
  padding-top: 1rem;
  padding-bottom: 1rem;
  justify-content: flex-start;
}
