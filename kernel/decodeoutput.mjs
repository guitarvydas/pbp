#!/usr/bin/env node
import fs from 'fs';

// Track files we've initialized (so we start each one fresh)
const initializedFiles = new Set();

// Buffer to store stdin data
let inputData = '';

// Read from stdin
process.stdin.on('data', (chunk) => {
  inputData += chunk.toString();
});

// Process the data when stdin ends
process.stdin.on('end', () => {
  try {
    // Parse the JSON input
    const jsonArray = JSON.parse(inputData);

    // Validate that the input is an array
    if (!Array.isArray(jsonArray)) {
      console.error('Error: Input is not a JSON array');
      process.exit(1);
    }

    // Process each object in the array
    jsonArray.forEach((obj) => {
      // Process each key/value pair
      for (const [key, value] of Object.entries(obj)) {
        // Ensure both key and value are strings
        if (typeof key !== 'string' || typeof value !== 'string') {
          console.error('Error: Keys and values must be strings');
          continue;
        }

        // Output file is derived directly from the key
        const outputFile = `out.${key}`;

        // Initialize (truncate) the file the first time we see this key
        if (!initializedFiles.has(outputFile)) {
          fs.writeFileSync(outputFile, '');
          initializedFiles.add(outputFile);
        }

        // Append the content to the appropriate file
        fs.appendFileSync(outputFile, value);
      }
    });

    // Add a trailing newline to every file that was written
    for (const outputFile of initializedFiles) {
      fs.appendFileSync(outputFile, '\n');
    }
  } catch (error) {
    console.error('Error processing input:', error.message);
    process.exit(1);
  }
});
