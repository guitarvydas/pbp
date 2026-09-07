function encodews (s) { return encodequotes (encodeURIComponent (s)); }

function encodequotes (s) { 
    let rs = s.replace (/"/g, '%22').replace (/'/g, '%27');
    return rs;
}

let linenumber = 0;
function getlineinc () {
    linenumber += 1;
    return `${linenumber}`;
}

function enspace (arr) {
    // create space-separated args for exec
    return arr;
    //return arr.join (" ");
}

// In Javascript:
// s is a string containing a two-level list.
// The top level items are separated by "⫶".
// Each inner item contains sub-items separated by "◦".
// The top level list always contains a trailing "⫶", resulting in an empty final top level item.
// example: s = "aaa◦bbb⫶ccc◦ddd⫶"
// Function `first(s)` .joins('') every first sub-item of every inner item.
// Function `second(s)` .joins('') every second sub-item of every inner item.
function first(s) {
  return s.split('⫶').slice(0, -1).map(item => item.split('◦')[0]).join('');
}

function second(s) {
  return s.split('⫶').slice(0, -1).map(item => item.split('◦')[1]).join('');
}

