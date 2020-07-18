const path = require('path');

module.exports = {
  entry: './src/Index.bs.js',
  output: {
    path: path.join(__dirname, "bundleOutput"),
    filename: 'index.js',
  },
  module: {
    rules: [
      {
        test: /\.js$/,
        exclude: /(node_modules|bower_components|lib)/,
        use: {
          loader: 'babel-loader',
          options: {
            presets: ['@babel/preset-env']
          }
        }
      }
    ]
  },
  devServer: {
    contentBase: path.join(__dirname, 'bundleOutput'),
    compress: true,
    port: 8000
  }
};