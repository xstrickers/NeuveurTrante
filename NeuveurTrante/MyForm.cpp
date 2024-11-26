#include "MyForm.h"
#include <al.h>
#include <alc.h>
#include <fstream>
#include <vector>
#include <string>


using namespace System;
using namespace System::Windows::Forms;
[STAThreadAttribute]
void Main(array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	NeuveurTrante form;
	Application::Run(% form);
}


// OpenAL variables
ALCdevice* device = nullptr;
ALCcontext* context = nullptr;
ALuint source;
ALuint buffer;
float volume = 0.5f;  // Volume initial à 50%
std::vector<std::string> playlist;

std::string SysStringToStd(System::String^ sysString) {
	using namespace System::Runtime::InteropServices;
	IntPtr ptr = Marshal::StringToHGlobalAnsi(sysString);
	std::string stdString = static_cast<char*>(ptr.ToPointer());
	Marshal::FreeHGlobal(ptr);
	return stdString;
}

// Initialisation d'OpenAL
void InitializeOpenAL() {
	device = alcOpenDevice(nullptr); // Périphérique par défaut
	if (!device) {
		MessageBox::Show("Impossible d'ouvrir le périphérique OpenAL.");
		return;
	}
	context = alcCreateContext(device, nullptr);
	if (!alcMakeContextCurrent(context)) {
		MessageBox::Show("Impossible de définir le contexte OpenAL.");
		return;
	}
	alGenSources(1, &source);
}

// Nettoyage des ressources OpenAL
void CleanupOpenAL() {
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

// Charger un fichier WAV dans un buffer OpenAL
bool LoadWavFile(const std::string& filename, ALuint& buffer) {
	// Vous devez ajouter la logique pour charger un fichier WAV ici
	// Ce code est un exemple générique et ne contient pas la logique de chargement du fichier
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open()) {
		return false;
	}
	// Code pour charger un fichier WAV dans un buffer OpenAL
	// Vous devrez probablement utiliser une bibliothèque comme stb_vorbis pour le décodage
	// ou implémenter votre propre logique ici

	file.close();
	return true;
}


void NeuveurTrante::OnPlay(System::Object^ sender, System::EventArgs^ e) {
	std::string filePath;
	if (playlistBox->SelectedItem) {
		filePath = SysStringToStd(playlistBox->SelectedItem->ToString());
	}
	//    // Charger et lire le fichier
	//    alSourceStop(source);
	//    alDeleteBuffers(1, &buffer);

	//    if (LoadWavFile(filePath, buffer)) {
	//        alSourcei(source, AL_BUFFER, buffer);
	//        alSourcePlay(source);
	//    }
	//    else {
	//        MessageBox::Show("Impossible de charger le fichier audio.");
	//    }
	//}

	ALuint      uiBuffer;
	ALuint      uiSource;
	ALint       iState;

	// Generate an AL Buffer
	alGenBuffers(1, &uiBuffer);

	// Load Wave file into OpenAL Buffer
	if (!ALFWLoadWaveToBuffer(filePath.c_str(), uiBuffer))
	{
		ALFWprintf("Failed to load %s\n", filePath);
	}

	// Generate a Source to playback the Buffer
	alGenSources(1, &uiSource);

	// Attach Source to Buffer
	alSourcei(uiSource, AL_BUFFER, uiBuffer);
	//pour boucler un son
	alSourcei(uiSource, AL_LOOPING, 1);
	// Play Source
	alSourcePlay(uiSource);
}

void NeuveurTrante::OnStop(System::Object^ sender, System::EventArgs^ e) {
	alSourceStop(source);
}

void NeuveurTrante::OnPause(System::Object^ sender, System::EventArgs^ e) {
	alSourcePause(source);
}

void NeuveurTrante::OnResume(System::Object^ sender, System::EventArgs^ e) {
	alSourcePlay(source);
}

void NeuveurTrante::OnVolumeChange(System::Object^ sender, System::EventArgs^ e) {
	volume = volumeTrackBar->Value / 100.0f;  // Convertir la valeur du trackbar en un pourcentage (0.0 - 1.0)
	alSourcef(source, AL_GAIN, volume);  // Appliquer la modification du volume
}

void NeuveurTrante::OnAddFile(System::Object^ sender, System::EventArgs^ e) {
	// Ouvrir une boîte de dialogue pour ajouter un fichier à la playlist
	OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
	openFileDialog->Filter = "Audio Files|*.wav;*.ogg|All Files|*.*";
	if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		std::string filePath = SysStringToStd(openFileDialog->FileName);
		playlist.push_back(filePath);
		playlistBox->Items->Add(openFileDialog->FileName);
	}
}

void NeuveurTrante::OnRemoveFile(System::Object^ sender, System::EventArgs^ e) {
	// Supprimer un fichier sélectionné de la playlist
	if (playlistBox->SelectedItem) {
		std::string filePath = SysStringToStd(playlistBox->SelectedItem->ToString());
		playlist.erase(std::remove(playlist.begin(), playlist.end(), filePath), playlist.end());
		playlistBox->Items->Remove(playlistBox->SelectedItem);
	}
}

void NeuveurTrante::OnSavePlaylist(System::Object^ sender, System::EventArgs^ e) {
	// Enregistrer la playlist dans un fichier
	SaveFileDialog^ saveFileDialog = gcnew SaveFileDialog();
	saveFileDialog->Filter = "Text Files|*.txt|All Files|*.*";
	if (saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		std::ofstream outFile(SysStringToStd(saveFileDialog->FileName));
		for (const std::string& file : playlist) {
			outFile << file << std::endl;
		}
		outFile.close();
	}
}

void NeuveurTrante::OnLoadPlayList(System::Object^ sender, System::EventArgs^ e) {
	// Charger la playlist à partir d'un fichier
	OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
	openFileDialog->Filter = "Text Files|*.txt|All Files|*.*";
	if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		std::ifstream inFile(SysStringToStd(openFileDialog->FileName));
		playlist.clear();
		playlistBox->Items->Clear();
		std::string line;
		while (std::getline(inFile, line)) {
			playlist.push_back(line);
			playlistBox->Items->Add(gcnew System::String(line.c_str()));
		}
		inFile.close();
	}
}



// Nettoyer OpenAL à la fermeture de l'application
void NeuveurTrante::CleanupOpenAL() {
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

