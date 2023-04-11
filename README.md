# NanoOcp

NanoOcp is a code collection based on JUCE framework including means to set up a minimal AES70 OCP.1 connection via tcp and handle/parse the protocol data in a basic way.

## Code contents

### NanoOcp1

... contains tcp client/server class implementations that can be instantiated and used for protocol communication.

### Ocp1Message

... contains protocol structure implementations required to interpret the data that is provided by NanoOcp1 client/server classes

### Ocp1ObjectDefinitions

... contains specific 'usecase' object definitions that can be used in combination with Ocp1Message

## Example project NanoOcp1Demo

This subfolder contains a JUCE framework project demonstrating how the NanoOcp1 classes and structures can be used in an operational application.