# Streaming Protocol

- All messages are byte-formated.

## Byte-code definition:

CLIENT_FRAME 10

## Variables
client_id: Id of client.

## Video Uploading
App -> Server:  <CLIENT_FRAME>< client_id > <client_auth_key> < frame_info > <pay_load>
               |...1 byte...||...1 byte... |  |...4byte...|    |...10byte...|