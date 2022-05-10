#!/bin/bash

set -e -o pipefail

# Takes the input file sentences.txt and saves .mp3 files in the data folder
# requires ffmpeg and jq installed

# Google TTS api key
KEY=AIzaSyCtACC9XGTVwjzq2bW9OMX5FtAZxD3ee0U
dir="data/HWTestSamples"

[ ! -f "sentences.txt" ] && echo "Execute this script from project root." && exit 1

rm -rf "$dir"
mkdir -p "$dir"

while IFS= read -r sentence; do
  name=$(echo "$sentence" | cut -d " " -f1)
  sentence=$(echo "$sentence" | tail -c +$(expr ${#name} + 2))
  echo "$dir/$name: $sentence"
  curl -s --location --request POST "https://texttospeech.googleapis.com/v1/text:synthesize?key=$KEY" \
    --header "Content-Type: application/json" \
    --data-raw '{
    "input": { "ssml": "<speak>'"$sentence"'</speak>" },
    "voice": {
        "languageCode": "en-US",
        "name": "en-US-Standard-D",
        "ssmlGender": "NEUTRAL"
    },
    "audioConfig": {
        "audioEncoding": "MP3",
        "speakingRate": 1.05,
        "pitch": 5.5,
        "sampleRateHertz": 24000
    }
  }' | jq -r '.audioContent' | base64 -d  | ffmpeg -loglevel quiet -f mp3 -i pipe: -af "apad=pad_dur=0.5" -codec:a aac -b:a 64k -ac 1 -ar 24000 "$dir/$name.aac"
done < sentences.txt
