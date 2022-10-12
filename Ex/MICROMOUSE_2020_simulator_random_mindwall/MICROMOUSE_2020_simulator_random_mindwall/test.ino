void test() {
  

  maze.addtestwalls();
    maze.solve();
    maze.print();

    Serial.println(maze.values[mouseRow][mouseColumn]);
}
