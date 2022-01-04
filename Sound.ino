void SetupSound() 
{
  mp3.begin();
  uint16_t volume = mp3.getVolume();
//  Serial.print("volume ");
//  Serial.println(volume);
  mp3.setVolume(24);
  
  uint16_t count = mp3.getTotalTrackCount(DfMp3_PlaySource_Sd);
//  Serial.print("files ");
//  Serial.println(count);

  uint16_t mode = mp3.getPlaybackMode();
//  Serial.print("playback mode ");
//  Serial.println(mode);
//  
//  Serial.println("starting...");
  
  mp3.playRandomTrackFromAll(); // random of all folders on sd
}

void UpdateSound()
{
    mp3.loop(); // just keeps playing the random tracks
//  Serial.println("track 1"); 
//  mp3.playMp3FolderTrack(1);  // sd:/mp3/0001.mp3
//  
//  waitMilliseconds(5000);
//  
//  Serial.println("track 2"); 
//  mp3.playMp3FolderTrack(2); // sd:/mp3/0002.mp3
//  
//  waitMilliseconds(5000);
//  
//  Serial.println("track 3");
//  mp3.playMp3FolderTrack(3); // sd:/mp3/0002.mp3
//  
//  waitMilliseconds(5000); 
}

void waitMilliseconds(uint16_t msWait)
{
  uint32_t start = millis();
  
  while ((millis() - start) < msWait)
  {
    // calling mp3.loop() periodically allows for notifications 
    // to be handled without interrupts
    mp3.loop(); 
    delay(1);
  }
}
