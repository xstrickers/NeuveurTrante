#include "MyForm.h"
#include <al.h>
#include <alc.h>
#include <fstream>
#include <vector>
#include <string>

using namespace System;
using namespace System::Windows::Forms;
[STAThreadAttribute]

void Main(array<String^>^ args) 
{
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

std::string NeuveurTrante::SysStringToStd(System::String^ sysString)
{
	using namespace System::Runtime::InteropServices;
	IntPtr ptr = Marshal::StringToHGlobalAnsi(sysString);
	std::string stdString = static_cast<char*>(ptr.ToPointer());
	Marshal::FreeHGlobal(ptr);
	return stdString;
}

// Initialisation d'OpenAL
//void InitializeOpenAL() {
//	device = alcOpenDevice(nullptr); // Périphérique par défaut
//	if (!device) {
//		MessageBox::Show("Impossible d'ouvrir le périphérique OpenAL.");
//		return;
//	}
//	context = alcCreateContext(device, nullptr);
//	if (!alcMakeContextCurrent(context)) {
//		MessageBox::Show("Impossible de définir le contexte OpenAL.");
//		return;
//	}
//	alGenSources(1, &source);
//}

// Nettoyage des ressources OpenAL
void CleanupOpenAL() 
{
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

// Charger un fichier WAV dans un buffer OpenAL
bool NeuveurTrante::LoadWavFile(const std::string& filename, ALuint& buffer) 
{
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

// Charger un fichier OGG
//bool NeuveurTrante::LoadOggFile(const std::string& filename, ALuint& buffer) 
//{
//	int channels, sampleRate;
//	short* output;
//	int samples = stb_vorbis_decode_filename(filename.c_str(), &channels, &sampleRate, &output);
//	if (samples < 0) {
//		return false;
//	}
//
//	alGenBuffers(1, &buffer);
//	alBufferData(buffer, (channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, output,
//		samples * sizeof(short), sampleRate);
//	free(output);
//	return true;
//}

void NeuveurTrante::OnPlay(System::Object^ sender, System::EventArgs^ e) 
{
	std::string filePath;
	if (playlistBox->SelectedItem) 
	{
		filePath = SysStringToStd(playlistBox->SelectedItem->ToString());
	}

	

	// Generate an AL Buffer
	alGenBuffers(1, &buffer);

	// Load Wave file into OpenAL Buffer
	if (!ALFWLoadWaveToBuffer(filePath.c_str(), buffer))
	{
		ALFWprintf("Failed to load %s\n", filePath);
	}

	// Generate a Source to playback the Buffer
	alGenSources(1, &source);

	// Attach Source to Buffer
	alSourcei(source, AL_BUFFER, buffer);
	//pour boucler un son
	alSourcei(source, AL_LOOPING, 1);
	// Play Source
	alSourcePlay(source);
}

void NeuveurTrante::OnStop(System::Object^ sender, System::EventArgs^ e) 
{
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);

	if (state == AL_PLAYING || state == AL_PAUSED) 
	{
		alSourceStop(source);
		alSourceRewind(source); // Remet la lecture au début
	}
	else {
		MessageBox::Show("Aucun son à arrêter.");
	}
}


void NeuveurTrante::OnPause(System::Object^ sender, System::EventArgs^ e) 
{
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);

	if (state == AL_PLAYING) 
	{
		alSourcePause(source);
	}
	else {
		MessageBox::Show("Aucun son en cours de lecture.");
	}
}


void NeuveurTrante::OnResume(System::Object^ sender, System::EventArgs^ e) 
{
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);

	if (state == AL_PAUSED) 
	{ 
		alSourcePlay(source);
	}
	else {
		MessageBox::Show("Impossible de reprendre, la source n'est pas en pause.");
	}
}


void NeuveurTrante::OnVolumeChange(System::Object^ sender, System::EventArgs^ e) 
{
	volume = volumeTrackBar->Value / 100.0f;  // Convertir la valeur du trackbar en un pourcentage (0.0 - 1.0)
	alSourcef(source, AL_GAIN, volume);  // Appliquer la modification du volume
}

void NeuveurTrante::OnAddFile(System::Object^ sender, System::EventArgs^ e) {
	OpenFileDialog^ dialog = gcnew OpenFileDialog();
	dialog->Filter = "Audio Files (*.wav;*.ogg)|*.wav;*.ogg";
	dialog->Title = "Sélectionnez un fichier audio";

	if (dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		std::string filePath = SysStringToStd(dialog->FileName);
		ALuint tempBuffer;

		// Charger le fichier audio selon son extension
		if (EndsWith(filePath, ".wav")) {
			if (!LoadWavFile(filePath, tempBuffer)) {
				MessageBox::Show("Erreur de chargement du fichier WAV.");
				return;
			}
		}
		/*else if (EndsWith(filePath, ".ogg")) {
			if (!LoadOggFile(filePath, tempBuffer)) {
				MessageBox::Show("Erreur de chargement du fichier OGG.");
				return;
			}
		}*/
		else {
			MessageBox::Show("Format non supporté.");
			return;
		}

		// Ajouter le fichier valide à la playlist
		playlist.push_back(filePath);
		playlistBox->Items->Add(dialog->FileName);
	}
}



void NeuveurTrante::OnRemoveFile(System::Object^ sender, System::EventArgs^ e) 
{
	// Supprimer un fichier sélectionné de la playlist
	if (playlistBox->SelectedItem) 
	{
		std::string filePath = SysStringToStd(playlistBox->SelectedItem->ToString());
		playlist.erase(std::remove(playlist.begin(), playlist.end(), filePath), playlist.end());
		playlistBox->Items->Remove(playlistBox->SelectedItem);
	}
}

void NeuveurTrante::OnSavePlaylist(System::Object^ sender, System::EventArgs^ e) 
{
	// Enregistrer la playlist dans un fichier
	SaveFileDialog^ saveFileDialog = gcnew SaveFileDialog();
	saveFileDialog->Filter = "Text Files|*.txt|All Files|*.*";
	if (saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) 
	{
		std::ofstream outFile(SysStringToStd(saveFileDialog->FileName));
		for (const std::string& file : playlist) 
		{
			outFile << file << std::endl;
		}
		outFile.close();
	}
}

void NeuveurTrante::OnLoadPlayList(System::Object^ sender, System::EventArgs^ e) 
{
	// Charger la playlist à partir d'un fichier
	OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
	openFileDialog->Filter = "Text Files|*.txt|All Files|*.*";
	if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) 
	{
		std::ifstream inFile(SysStringToStd(openFileDialog->FileName));
		playlist.clear();
		playlistBox->Items->Clear();
		std::string line;
		while (std::getline(inFile, line)) 
		{
			playlist.push_back(line);
			playlistBox->Items->Add(gcnew System::String(line.c_str()));
		}
		inFile.close();
	}
}


bool NeuveurTrante::EndsWith(const std::string& str, const std::string& suffix) 
{
	if (str.size() < suffix.size()) return false;
	return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

// Nettoyer OpenAL à la fermeture de l'application
void NeuveurTrante::CleanupOpenAL() {
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

