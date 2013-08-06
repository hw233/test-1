update player set gmLevel = 0 where gmLevel != 0 and id&0xffffffffff != 5;
