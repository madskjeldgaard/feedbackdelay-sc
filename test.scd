(
s.waitForBoot{
    // Ndef(\delaytest,{|delayTime=0.5, feedback=0.5, crossfade=0, lowpass=0.1, highpass=0.1|
    //     var maxDelayTime = 30;
    //     var input = SoundIn.ar(0) * 4;
    //     var delayed = FeedbackDelay.ar(input, maxDelayTime, delayTime, feedback*2, lowpass, highpass);
    //     XFade2.ar(input, delayed, crossfade)
    // }).play;

    Ndef(\filterTest,{
        var maxDelayTime = 30;
        var input = WhiteNoise.ar(0.5);
        var cutoff = \cutoff.ar(1500, 0.1, spec: [50,15000,\exp]);
        var res = \res.ar(0.5, 0.1, spec: [0.1,1.0,\lin]);
        var type = \type.ar(0, 0.1, spec: [0,7,\lin]);

        VAStateVariableFilter.ar(input, cutoff, res, type)!2
        // input
    }).play;

    Ndef(\filterTest).gui2;

    // Ndef(\delaytest,{|crossfade=0|
    //     var maxDelayTime = 30;
    //     var input = SoundIn.ar(0) * 2;
    //     var delayTime = LFNoise2.ar(10).range(0.01,1.0);
    //     var feedback = LFNoise2.ar(1).range(0.7,0.95);
    //     var delayed = FeedbackDelay.ar(input, maxDelayTime, delayTime, feedback);
    //     XFade2.ar(input, delayed, crossfade)
    // }).play;

    // Ndef(\delaytest).gui2;
}
)
