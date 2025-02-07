(
s.waitForBoot{
    Ndef(\delaytest,{|delayTime=0.5, feedback=0.5, crossfade=0|
        var maxDelayTime = 30;
        var input = SoundIn.ar(0) * 2;
        var delayed = FeedbackDelay.ar(input, maxDelayTime, delayTime, feedback);
        XFade2.ar(input, delayed, crossfade)
    }).play;

    // Ndef(\delaytest,{|crossfade=0|
    //     var maxDelayTime = 30;
    //     var input = SoundIn.ar(0) * 2;
    //     var delayTime = LFNoise2.ar(10).range(0.01,1.0);
    //     var feedback = LFNoise2.ar(1).range(0.7,0.95);
    //     var delayed = FeedbackDelay.ar(input, maxDelayTime, delayTime, feedback);
    //     XFade2.ar(input, delayed, crossfade)
    // }).play;

    Ndef(\delaytest).gui2;
}
)
