import resolve from 'rollup-plugin-node-resolve';

export default {
  entry: 'lib/es6/src/sw.js',
  format: 'iife',
  plugins: [ resolve() ],
  dest: 'sw.js'
};
