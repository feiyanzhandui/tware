Alex's Chatroom README

Features
---------

* Homepage ("/")
   * Most Active Rooms - Displays the 5 most active chatrooms with the name and the number of hours ago the last message was posted
   * Most Active Users - Displays the 3 most active users across all rooms, with the number of posts the user has made
   * Users can click on an existing chatroom by clicking on one of the "Most Active Rooms"
   * Users can create a new room by clicking "Create New Room"

* Room
   * Message's div is independently scrollable so page isn't very long.
   * Users are prompted to enter their username upon entering a room
   * Users are forced to enter a valid username (no null or empty strings)
   * Users can enter a message that gets sent to entire room IN REAL TIME :)
   * When a user sends a message, the message appears instantly for that user and is sent to all other users
   * Users can go back to the home page via button on top left corner
   * Users can see other members in the room
   * LIVE TYPING! -- Users can see when another member is typing a message (pencil appears next to user's name in "Users" box)
   * Users are alerted when a user leaves the room
   * Messages are still stored in database so a new user can see old messages

* Other Notes
   * Templating engine Hogan used for HTML (room and index)
   * Parameterized SQL used for all user inputted strings

* Bugs
   * The only known bug is caused when a user picks a nickname that is already taken in the room that he/she enters
