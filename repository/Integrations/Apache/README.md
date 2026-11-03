# Integrations.Apache
*Integrations.Apache* is a CGI program for the *Apache web server*.

## Building and Installing under Windows
- *Integrations.Apache* is built with th rest of the repository.
  See [/./../../README.md](./../../README.md) for information on building this repository.
  Obviously, *Integrations.Apache* requires the *Apache* web server.
  A distribution of the *Apache* web server is, for example, *XAMPP*.

- Find the binary `arcadia.exe`
- Copy the binary binary to the `cgi-bin` directory of your Apache server.

- Add the handler to the `httpd.conf` of your Apache server.
  ```
  <IfModule mime_module>
      ...
      # Arcadia Apache integration.
      AddHandler arcadia .a
      Action arcadia "/cgi-bin/arcadia.exe"
      ...
  </IfModule>
  ```

- Copy the `index.a` file from the `Getting Started` directory in this repository in your `htdocs` folder.
  Ensure that your Apache runs and is reachable under `localhost`. Open your browser and open `localhost/index.a`.
  You should be greeted by the message
  ```
  Welcome to Arcadia.
  ```

## Building and Installing under Linux
- *Integrations.Apache* is built with th rest of the repository.
  See [/./../../README.md](./../../README.md) for information on building this repository.
  Obviously, *Integrations.Apache* requires the *Apache* web server.

- Find the binary `arcadia`
- Copy the binary binary to the `cgi-bin` directory of your Apache server.
- Add the handler to the `httpd.conf` of your Apache server.
  ```
  <IfModule mime_module>
      ...
      # Arcadia Apache integration.
      AddHandler arcadia .a
      Action arcadia "/cgi-bin/arcadia"
      ...
  </IfModule>
  ```
- Copy the `index.a` file from the `Getting Started` directory in this repository in your `htdocs` folder.
  Ensure that your Apache runs and is reachable under `localhost`. Open your browser and open `localhost/index.a`.
  You should be greeted by the message
  ```
  Welcome to Arcadia.
  ```
