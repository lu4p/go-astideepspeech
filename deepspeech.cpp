#include <stdio.h>
#include <deepspeech.h>

extern "C" {
    class ModelWrapper {
        private:
            ModelState* model;

        public:
            ModelWrapper(const char* aModelPath, int aNCep, int aNContext, const char* aAlphabetConfigPath, int aBeamWidth)
            {
                DS_CreateModel(aModelPath, aNCep, aNContext, aAlphabetConfigPath, aBeamWidth, &model);
            }

            ~ModelWrapper()
            {
                DS_DestroyModel(model);
            }

            void enableDecoderWithLM(const char* aAlphabetConfigPath, const char* aLMPath, const char* aTriePath, float aLMWeight, float aValidWordCountWeight)
            {
                DS_EnableDecoderWithLM(model, aAlphabetConfigPath, aLMPath, aTriePath, aLMWeight, aValidWordCountWeight);
            }

            char* stt(const short* aBuffer, unsigned int aBufferSize, unsigned int aSampleRate)
            {
                return DS_SpeechToText(model, aBuffer, aBufferSize, aSampleRate);
            }

            Metadata* sttWithMetadata(const short* aBuffer, unsigned int aBufferSize, unsigned int aSampleRate)
            {
                return DS_SpeechToTextWithMetadata(model, aBuffer, aBufferSize, aSampleRate);
            }

            ModelState* getModel()
            {
                return model;
            }
    };

    ModelWrapper* New(const char* aModelPath, int aNCep, int aNContext, const char* aAlphabetConfigPath, int aBeamWidth)
    {
        return new ModelWrapper(aModelPath, aNCep, aNContext, aAlphabetConfigPath, aBeamWidth);
    }
    void Close(ModelWrapper* w)
    {
        delete w;
    }

    void EnableDecoderWithLM(ModelWrapper* w, const char* aAlphabetConfigPath, const char* aLMPath, const char* aTriePath, float aLMWeight, float aValidWordCountWeight)
    {
        w->enableDecoderWithLM(aAlphabetConfigPath, aLMPath, aTriePath, aLMWeight, aValidWordCountWeight);
    }

    char* STT(ModelWrapper* w, const short* aBuffer, unsigned int aBufferSize, int aSampleRate)
    {
        return w->stt(aBuffer, aBufferSize, aSampleRate);
    }

    Metadata* STTWithMetadata(ModelWrapper* w, const short* aBuffer, unsigned int aBufferSize, int aSampleRate)
    {
        return w->sttWithMetadata(aBuffer, aBufferSize, aSampleRate);
    }

    double Metadata_GetProbability(Metadata* m)
    {
        return m->probability;
    }

    int Metadata_GetNumItems(Metadata* m)
    {
        return m->num_items;
    }

    MetadataItem* Metadata_GetItems(Metadata* m)
    {
        return m->items;
    }

    char* MetadataItem_GetCharacter(MetadataItem* mi)
    {
        return mi->character;
    }

    int MetadataItem_GetTimestep(MetadataItem* mi)
    {
        return mi->timestep;
    }

    float MetadataItem_GetStartTime(MetadataItem* mi)
    {
        return mi->start_time;
    }

    class StreamWrapper {
        private:
            StreamingState* s;

        public:
            StreamWrapper(ModelWrapper* w, unsigned int aPreAllocFrames, unsigned int aSampleRate)
            {
                DS_SetupStream(w->getModel(), aPreAllocFrames, aSampleRate, &s);
            }

            ~StreamWrapper()
            {
                DS_DiscardStream(s);
            }

            void feedAudioContent(const short* aBuffer, unsigned int aBufferSize)
            {
                DS_FeedAudioContent(s, aBuffer, aBufferSize);
            }

            char* intermediateDecode()
            {
                return DS_IntermediateDecode(s);
            }

            char* finishStream()
            {
                return DS_FinishStream(s);
            }

            Metadata* finishStreamWithMetadata()
            {
                return DS_FinishStreamWithMetadata(s);
            }

            void discardStream()
            {
                DS_DiscardStream(s);
            }
    };

    StreamWrapper* SetupStream(ModelWrapper* mw, unsigned int aPreAllocFrames, unsigned int aSampleRate)
    {
        return new StreamWrapper(mw, aPreAllocFrames, aSampleRate);
    }
    void DiscardStream(StreamWrapper* sw)
    {
        delete sw;
    }

    void FeedAudioContent(StreamWrapper* sw, const short* aBuffer, unsigned int aBufferSize)
    {
        sw->feedAudioContent(aBuffer, aBufferSize);
    }

    char* IntermediateDecode(StreamWrapper* sw)
    {
        return sw->intermediateDecode();
    }

    char* FinishStream(StreamWrapper* sw)
    {
        return sw->finishStream();
    }

    Metadata* FinishStreamWithMetadata(StreamWrapper* sw)
    {
        return sw->finishStreamWithMetadata();
    }

    void FreeString(char* s)
    {
        DS_FreeString(s);
    }

    void FreeMetadata(Metadata* m)
    {
        DS_FreeMetadata(m);
    }

    void PrintVersions()
    {
        DS_PrintVersions();
    }
}
