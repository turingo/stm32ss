#include "utils.h"
#include "leds.h"
#include "bz.h"

/*! Config 120 beats per minute !*/
#define BEAT_COUNT      (250 * 1000)

#define freq2us(freq)   (1000 * 1000 / 2 / freq)
#define note2id(note)   ((note < 0) ? (-1 - note) : (note + 6))

static const int note_units[] =
{
    freq2us(262) * 2,   /* l do */
    freq2us(294) * 2,   /* l re */
    freq2us(330) * 2,   /* l mi */
    freq2us(349) * 2,   /* l fa */
    freq2us(392) * 2,   /* l so */
    freq2us(440) * 2,   /* l la */
    freq2us(494) * 2,   /* l si */
    freq2us(262),       /* do */
    freq2us(294),       /* re */
    freq2us(330),       /* mi */
    freq2us(349),       /* fa */
    freq2us(392),       /* so */
    freq2us(440),       /* la */
    freq2us(494),       /* si */
};

/*! The notation of two tigers !*/
static const int notation[] =
{
    1, 2, 3, 1,
    1, 2, 3, 1,
    3, 4, 5,
    3, 4, 5,
    5, 6, 5, 4, 3, 1,
    5, 6, 5, 4, 3, 1,
    2, -5, 1,
    2, -5, 1,
};

/*! Two tigers playing demo !*/
void two_tigers(void)
{
    int nt_len = sizeof(notation) / sizeof(notation[0]);
    int nt_id = 0;
    int nt_unit;
    int i;

    leds_init();
    bz_init();

    for(;;)
    {
        /** Stop phase */
        leds_ctrl(LEDS_RED, LEDS_TURN_OFF);
        bz_ctrl(BZ_TURN_OFF);
        delayus(BEAT_COUNT);

        nt_unit = note_units[note2id(notation[nt_id])];
        if(++nt_id >= nt_len) nt_id = 0;

        /** Note phase */
        leds_ctrl(LEDS_RED, LEDS_TURN_ON);
        bz_ctrl(BZ_TURN_ON);
        for(i = 0; i < BEAT_COUNT; i += nt_unit * 2)
        {
            delayus(nt_unit);
            bz_ctrl(BZ_TOGGLE);
            delayus(nt_unit);
            bz_ctrl(BZ_TOGGLE);
        }
    }
}
