#pragma once

#include <iostream>
#include <vector>
#include <string>

namespace NeuveurTrante {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    public ref class NeuveurTrante : public System::Windows::Forms::Form
    {
    public:
        NeuveurTrante(void)
        {
            InitializeComponent();
        }

    protected:
        ~NeuveurTrante()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        System::Windows::Forms::Button^ playButton;
        System::Windows::Forms::Button^ stopButton;
        System::Windows::Forms::Button^ pauseButton;
        System::Windows::Forms::Button^ resumeButton;
        System::Windows::Forms::TrackBar^ volumeTrackBar;
        System::Windows::Forms::ListBox^ playlistBox;
        System::Windows::Forms::Button^ addFileButton;
        System::Windows::Forms::Button^ removeFileButton;
        System::Windows::Forms::Button^ savePlaylistButton;
        System::Windows::Forms::Button^ loadPlaylistButton;

    private:
        System::ComponentModel::Container^ components;

    private:
        void InitializeComponent(void)
        {
            this->playButton = (gcnew System::Windows::Forms::Button());
            this->stopButton = (gcnew System::Windows::Forms::Button());
            this->pauseButton = (gcnew System::Windows::Forms::Button());
            this->resumeButton = (gcnew System::Windows::Forms::Button());
            this->volumeTrackBar = (gcnew System::Windows::Forms::TrackBar());
            this->playlistBox = (gcnew System::Windows::Forms::ListBox());
            this->addFileButton = (gcnew System::Windows::Forms::Button());
            this->removeFileButton = (gcnew System::Windows::Forms::Button());
            this->savePlaylistButton = (gcnew System::Windows::Forms::Button());
            this->loadPlaylistButton = (gcnew System::Windows::Forms::Button());

            // Form properties
            this->SuspendLayout();
            this->Text = L"Audio Player - WAV & OGG";
            this->Width = 800;
            this->Height = 600;

            // Play Button
            this->playButton->Text = L"Play";
            this->playButton->Location = System::Drawing::Point(20, 20);
            this->playButton->Size = System::Drawing::Size(80, 30);

            // Stop Button
            this->stopButton->Text = L"Stop";
            this->stopButton->Location = System::Drawing::Point(110, 20);
            this->stopButton->Size = System::Drawing::Size(80, 30);

            // Pause Button
            this->pauseButton->Text = L"Pause";
            this->pauseButton->Location = System::Drawing::Point(200, 20);
            this->pauseButton->Size = System::Drawing::Size(80, 30);

            // Resume Button
            this->resumeButton->Text = L"Resume";
            this->resumeButton->Location = System::Drawing::Point(290, 20);
            this->resumeButton->Size = System::Drawing::Size(80, 30);

            // Volume TrackBar
            this->volumeTrackBar->Minimum = 0;
            this->volumeTrackBar->Maximum = 100;
            this->volumeTrackBar->Value = 50;
            this->volumeTrackBar->Location = System::Drawing::Point(20, 70);
            this->volumeTrackBar->Size = System::Drawing::Size(350, 50);

            // Playlist Box
            this->playlistBox->Location = System::Drawing::Point(20, 150);
            this->playlistBox->Size = System::Drawing::Size(400, 300);

            // Add File Button
            this->addFileButton->Text = L"Add File";
            this->addFileButton->Location = System::Drawing::Point(450, 150);
            this->addFileButton->Size = System::Drawing::Size(100, 30);

            // Remove File Button
            this->removeFileButton->Text = L"Remove File";
            this->removeFileButton->Location = System::Drawing::Point(450, 200);
            this->removeFileButton->Size = System::Drawing::Size(100, 30);

            // Save Playlist Button
            this->savePlaylistButton->Text = L"Save Playlist";
            this->savePlaylistButton->Location = System::Drawing::Point(450, 250);
            this->savePlaylistButton->Size = System::Drawing::Size(100, 30);

            // Load Playlist Button
            this->loadPlaylistButton->Text = L"Load Playlist";
            this->loadPlaylistButton->Location = System::Drawing::Point(450, 300);
            this->loadPlaylistButton->Size = System::Drawing::Size(100, 30);

            // Adding controls to the form
            this->Controls->Add(this->playButton);
            this->Controls->Add(this->stopButton);
            this->Controls->Add(this->pauseButton);
            this->Controls->Add(this->resumeButton);
            this->Controls->Add(this->volumeTrackBar);
            this->Controls->Add(this->playlistBox);
            this->Controls->Add(this->addFileButton);
            this->Controls->Add(this->removeFileButton);
            this->Controls->Add(this->savePlaylistButton);
            this->Controls->Add(this->loadPlaylistButton);

            this->ResumeLayout(false);
            this->PerformLayout();
        }
    };
}
