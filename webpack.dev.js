const path = require('path');
const config = require("./webpack.common");

module.exports = {
  ...config,
  mode: "development",
  devServer: {
    contentBase: path.join(__dirname),
    compress: true,
    port: 8000
  }
};