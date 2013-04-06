/*
 * Sound.h
 *
 *  Created on: Mar 31, 2012
 *      Author: CoM
 */

class CGameSound
{
	sf::Music ambientMusic;
	sf::SoundBuffer Buffer;
public:
	CGameSound(void);

	bool initSound(void);
	bool loadAmbient(const char* soundFile);
	void playAmbient(void);
}MainGameSound;

CGameSound::CGameSound(void)
{

}

bool CGameSound::initSound(void)
{
	return true;
	return false;
}

bool CGameSound::loadAmbient(const char* soundFile)
{
	if(!ambientMusic.OpenFromFile(soundFile))
	 {
		 cout << EXC << "Failed to load: " << soundFile;
		 return false;
	 }
	 else
	 {
		 cout << INS << "Successfully to loaded: " << soundFile;
		 return true;
	 }

	/*
    if (Buffer.LoadFromFile("ambient.mp3"))
	 {
		 cout << INS << "Successfully to loaded: " << soundFile;
		 return true;
	 }
	 else
	 {
		 cout << EXC << "Failed to load: " << soundFile;
		 return false;
	 }
	 */
}

void CGameSound::playAmbient(void)
{
	ambientMusic.Play();
/*
    sf::Sound Sound(Buffer);
    Sound.Play();
    */
}

/*
// Load a sound buffer from a wav file
    sf::SoundBuffer Buffer;
    if (!Buffer.LoadFromFile("datas/sound/footsteps.wav"))
        return;

    // Display sound informations
    std::cout << "footsteps.wav :" << std::endl;
    std::cout << " " << Buffer.GetDuration()      << " sec"           << std::endl;
    std::cout << " " << Buffer.GetSampleRate()    << " samples / sec" << std::endl;
    std::cout << " " << Buffer.GetChannelsCount() << " channels"      << std::endl;

    // Create a sound instance and play it
    sf::Sound Sound(Buffer);
    Sound.Play();
    */
