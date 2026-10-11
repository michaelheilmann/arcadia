/* Copyright (c) 2024-2025 Michael Heilmann. All rights reserved. */

html.my-theme-1 {
  color: var(--html-text-colork);
  background-color: var(--html-background-color);
  & code {
    color: var(--html-text-color);
  }
  & a {
    color: var(--html-link-color);
  }
  & body.my-index-page-1, body.my-content-page-1, & body.my-subindex-page-1, & body.my-error-page-1 {
    & > header {
      background-color: var(--header-background-color);
      color: var(--header-text-color);
      & a {
        color: var(--header-link-color);
      }
    }
    & > footer {
      background-color: var(--footer-background-color);
      color: var(--footer-text-color);
      & a {
        color: var(--footer-link-color);
      }
    }
  }
}
