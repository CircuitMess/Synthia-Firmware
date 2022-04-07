#include "SaveManager.h"

SaveManager saveManager;

SaveData SaveManager::load(uint8_t trackSlot, bool saveLastEdited){
	const String rootPath = "/Save/" + String(trackSlot) + "/";

	//clean current Recordings folder
	File recordings = SPIFFS.open("/Recordings/");
	clearFolder(recordings);

	//open track save saveFile
	String path = rootPath + "data.sav";
	File saveFile = SPIFFS.open(path);
	if(!saveFile){
		ESP_LOGE("SaveManager", "Couldn't open save saveFile: %s", path.c_str());
		return SaveData{};
	}
	SaveData data;
	size_t read = saveFile.read((uint8_t*)&data, sizeof(SaveData));
	if(read != sizeof(SaveData)){
		ESP_LOGE("SaveManager", "Couldn't read save saveFile: %d", read);
		return SaveData{};
	}
	saveFile.close();

	//copy over recordings from saved track
	File saveFolder = SPIFFS.open(rootPath);
	copyFolder(saveFolder, recordings);

	saveFolder.close();
	recordings.close();

	if(saveLastEdited){
		saveLast(trackSlot);
	}

	return data;
}

void SaveManager::store(uint8_t trackSlot, SaveData data, bool saveLastEdited){
	const String rootPath = "/Save/" + String(trackSlot) + "/";

	File saveFolder = SPIFFS.open(rootPath);
	clearFolder(saveFolder);


	File savedata = SPIFFS.open(rootPath + "data.sav", "w");
	savedata.write((uint8_t*)&data, sizeof(SaveData));
	savedata.close();

	File recordings = SPIFFS.open("/Recordings/");
	copyFolder(recordings, saveFolder);

	if(saveLastEdited){
		saveLast(trackSlot);
	}
}

void SaveManager::copyFile(File& source, File& destination){
	auto copyBuf = (uint8_t*)malloc(4096);
	while(source.available()){
		size_t read = source.read(copyBuf, 4096);
		destination.write(copyBuf, read);
	}
	free(copyBuf);
}

void SaveManager::clearFolder(File& folder){
	if(!folder.isDirectory())return;

	File file = SPIFFS.open(String(folder.name()) + "data.sav");
	SPIFFS.remove(file.name());
	file.close();
	for(uint8_t i = 0; i < 5; i++){
		file = SPIFFS.open(String(folder.name()) + String(i) + ".aac");
		SPIFFS.remove(file.name());
		file.close();
	}
}

void SaveManager::copyFolder(File& source, File& destination){
	if(!(source.isDirectory() && destination.isDirectory())) return;

	File file;
	File copy;
	for(uint8_t i = 0; i < 5; i++){
		String path = String(source.name()) + String(i) + ".aac";
		if(!SPIFFS.exists(path)) continue;
		file = SPIFFS.open(path);
		copy = SPIFFS.open(String(destination.name()) + String(i) + ".aac", "w");
		copyFile(file, copy);
		file.close();
		copy.close();
	}
}

void SaveManager::saveLast(uint8_t trackSlot){
	File lastData = SPIFFS.open("/Save/last.bin", "w");
	lastData.write(trackSlot);
	lastData.close();
}

uint8_t SaveManager::getLast(){
	uint8_t data = 0;
	File lastData = SPIFFS.open("/Save/last.bin");
	if(lastData){
		lastData.read(&data, 1);
	}
	lastData.close();

	return data;
}

SaveData SaveManager::loadLast(){
	return load(getLast(), false);
}
