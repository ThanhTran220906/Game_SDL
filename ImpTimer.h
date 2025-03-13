
#pragma

#define FRAME_PER_SECOND 25
int const one_frame_time=1000/FRAME_PER_SECOND;

class ImpTimer
{
public:
    ImpTimer();
    ~ImpTimer();
    void game_start();

    int get_ticks();


private:
    int start_ticks;
    int pause_ticks;

    bool is_start;
    bool is_pause;
};
