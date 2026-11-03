# Cookie Bot Protection

This module is designed to provide protection from simple crawlers such as AI scrapers or crawling scripts, by testing their ability to follow redirects and handle cookies.

Many AI bots and non-conventional crawlers do not respect robots.txt and can cause significant performance issues on pages with heavy computation, such as search pages.

The module will not fully block bot traffic or force timeouts, but it will significantly reduce their impact by quickly returning a 302 or 401 response without computing the page, which is not served from Page Cache.

You can define URL patterns subject to the protection using PCRE regex patterns. Use non-capturing groups in your patterns, as they are tested with preg_match(). Poorly written regex rules can accidentally protect the entire site and affect SEO
crawlers.

We recommend applying the same patterns as your robots.txt.

By default, a retry attempt is provided via a Refresh header to allow legitimate users to recover from an accidental challenge failure. This behavior can be disabled by setting the redirect error delay to zero.

A challenge cookie named SESScookiebotprotection is created to validate the challenge. This name is chosen to bypass generic Varnish/Fastly configurations for Drupal.

Specific IP addresses and User-Agent strings can be excluded from the challenge.

This module is intended as a lightweight tool for sites that cannot install a challenge-based WAF such as Anubis or go-away as a reverse proxy.

Because the cookie name is central to the project's identity, this module will not evolve to provide other protection mechanisms such as JavaScript Proof of Work or CAPTCHA challenges.

## Installation

Install this module using the [official Backdrop CMS instructions](https://backdropcms.org/guide/modules).

## Issues

Bugs and Feature requests should be reported in the [Issue Queue](https://github.com/backdrop-contrib/cookie_bot_protection/issues).

## Current Maintainers

- [Herb v/d Dool](https://github.com/herbdool)
- Co-maintainers are welcome.

## Credits

Ported from the Drupal module. Drupal maintainers:

- [macsim](https://www.drupal.org/u/macsim)
- [pilulu](https://www.drupal.org/u/pilulu)

Supporting org: https://www.drupal.org/axess-open-web-services

## License

This project is GPL v2 software.
See the LICENSE.txt file in this directory for complete text.
