#ifndef IMPTIMMER_H
#define IMPTIMMER_H
class impTimer{
public:
    impTimer();
    ~impTimer();

    void start();
    void stop();
    void paused();
    void unpaused();

    int get_ticks();

    bool is_started();
    bool is_paused();

private:
    int start_tick ;
    int paused_tick ;
    bool is_paused_ ;
    bool is_started_ ;
};
#endif // IMPTIMMER_H
