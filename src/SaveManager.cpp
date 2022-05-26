#include "SaveManager.h"
#include "AudioSystem/Transcoder.h"
#include "AudioSystem/PlaybackSystem.h"
#include "Visualization/RGBController.h"

SaveManager saveManager;

SaveData SaveManager::defaultData;

SaveManager::SaveManager(){
	for(int i = 0; i < 5; i++){
		defaultData.slots[i].sample.type = (Sample::Type) i;
		defaultData.slots[i].slotIndex = i;
	}
}

void SaveManager::begin(){
	File recordings = SPIFFS.open("/Recordings/");
	clearFolder(recordings);
}

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

		return defaultData;
	}
	SaveData data;
	size_t read = saveFile.read((uint8_t*)&data, sizeof(SaveData));
	if(read != sizeof(SaveData)){
		ESP_LOGE("SaveManager", "Couldn't read save saveFile: %lu", read);
		return defaultData;
	}
	saveFile.close();

	File saveFolder = SPIFFS.open(rootPath);

	Playback.stop();
	loadRecordings(saveFolder, recordings);
	Playback.begin();

	saveFolder.close();
	recordings.close();

	if(saveLastEdited){
		saveLast(trackSlot);
	}

	RGBSlot.clear();

	return data;
}

void SaveManager::store(uint8_t trackSlot, SaveData data, bool saveLastEdited){
	const String rootPath = "/Save/" + String(trackSlot) + "/";

	File saveFolder = SPIFFS.open(rootPath);
	clearFolder(saveFolder);

	File savedata = SPIFFS.open(rootPath + "data.sav", FILE_WRITE);
	savedata.write((uint8_t*)&data, sizeof(SaveData));
	savedata.close();

	File recordings = SPIFFS.open("/Recordings/");

	Playback.stop();
	storeRecordings(recordings, saveFolder);
	Playback.begin();

	if(saveLastEdited){
		saveLast(trackSlot);
	}

	RGBSlot.clear();
}

void SaveManager::copyFile(File& source, File& destination){
	auto copyBuf = (uint8_t*) malloc(4096);
	while(source.available()){
		size_t read = source.read(copyBuf, 4096);
		destination.write(copyBuf, read);
	}
	free(copyBuf);
}

void SaveManager::clearFolder(File& folder){
	if(!folder.isDirectory()) return;
	String path = folder.name();
	if(!path.endsWith("/")){
		path += "/";
	}

	SPIFFS.remove(path + "data.sav");
	for(uint8_t i = 0; i < 5; i++){
		SPIFFS.remove(path + i + ".wav");
		SPIFFS.remove(path + i + ".aac");
	}
}

void IRAM_ATTR SaveManager::loadRecordings(File& source, File& destination){
	if(!(source.isDirectory() && destination.isDirectory())) return;

	String srcDir = String(source.name());
	String dstDir = String(destination.name());

	if(!srcDir.endsWith("/")){
		srcDir += "/";
	}
	if(!dstDir.endsWith("/")){
		dstDir += "/";
	}

	File file;
	File copy;
	for(uint8_t i = 0; i < 5; i++){
		String srcPath = srcDir + i + ".aac";
		String dstPath = dstDir + i + ".wav";

		SPIFFS.remove(dstPath);

		RGBSlot.setSolid(i, MatrixPixel::Blue);

		file = SPIFFS.open(srcPath);
		if(!file){
			continue;
		}

		copy = SPIFFS.open(dstPath, FILE_WRITE);

		uint32_t time = millis();
		Transcoder transcoder(AAC2WAV, file, copy);
		transcoder.start();
		while(!transcoder.isDone()){
			delay(1);
		}
		ESP_LOGI("SaveManager", "Load transcode done in %d ms\n", millis() - time);
	}
}

void IRAM_ATTR SaveManager::storeRecordings(File& source, File& destination){
	if(!(source.isDirectory() && destination.isDirectory())) return;

	String srcDir = String(source.name());
	String dstDir = String(destination.name());

	if(!srcDir.endsWith("/")){
		srcDir += "/";
	}
	if(!dstDir.endsWith("/")){
		dstDir += "/";
	}

	File file;
	File copy;
	for(uint8_t i = 0; i < 5; i++){
		String srcPath = srcDir + i + ".wav";
		String dstPath = dstDir + i + ".aac";

		SPIFFS.remove(dstPath);

		RGBSlot.setSolid(i, MatrixPixel::Blue);

		file = SPIFFS.open(srcPath);
		if(!file){
			continue;
		}

		copy = SPIFFS.open(dstPath, FILE_WRITE);

		uint32_t time = millis();
		Transcoder transcoder(WAV2AAC, file, copy);
		transcoder.start();
		while(!transcoder.isDone()){
			delay(1);
		}
		ESP_LOGI("SaveManager", "Transcode done is %d ms\n", millis() - time);
	}
}

void SaveManager::copyFolder(File& source, File& destination){
	if(!(source.isDirectory() && destination.isDirectory())) return;

	String srcDir = String(source.name());
	String dstDir = String(destination.name());

	if(!srcDir.endsWith("/")){
		srcDir += "/";
	}
	if(!dstDir.endsWith("/")){
		dstDir += "/";
	}

	File file;
	File copy;
	for(uint8_t i = 0; i < 5; i++){
		String srcPath = srcDir + i + ".wav";
		String dstPath = dstDir + i + ".wav";

		SPIFFS.remove(dstPath);

		file = SPIFFS.open(srcPath);
		if(!file){
			continue;
		}

		copy = SPIFFS.open(dstPath, FILE_WRITE);

		copyFile(file, copy);
	}
}

void SaveManager::saveLast(uint8_t trackSlot){
	SPIFFS.remove("/Save/last.bin");
	File lastData = SPIFFS.open("/Save/last.bin", FILE_WRITE);
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
