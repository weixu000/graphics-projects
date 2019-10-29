#ifndef TIME_H
#define TIME_H


class Time {
public:
    Time() = delete;

    static void reset();

    static void tick();

    static float now() { return _now; }

    static float delta() { return _delta; }

private:
    static float _now;
    static float _delta;
};


#endif //TIME_H
