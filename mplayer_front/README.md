# MPLAYER_FRONT

A simple C program that uses a FIFO mechanism to control **MPlayer** for audio playback. It allows navigation through audio files using keyboard commands.

## Requirements
- **MPlayer** must be installed on your system.

## Usage
1. Start the frontend program:
   ```bash
   ./mplayer_front
2. In another terminal, start MPlayer with FIFO input:
    ```bash
    mplayer -input file=/tmp/mplayer_fifo <audio_file.mp3>
3. Use the following keyboard commands in the first terminal to control playback
- `p`: Pause/Play
- `+`: Seek forward 5 seconds
- `-`: Seek backward 5 seconds
- `q`: Quit