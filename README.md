# Final Stand

A Tower-defense game for teaching.

See live version at [https://final-stand.surge.sh/](https://final-stand.surge.sh/).

## Run

```sh
npm install
npm run compile
npm run webpack
```

We're using the `.re` syntax because development is difficult without editor
support. But just to see how it would look in the `.res` syntax, try `npm run res-fmt`.

To deploy, run `npm run deploy`.

(This tries to upload to a Surge site owned by Jasim, so that can fail. Use a
different domain then.)
