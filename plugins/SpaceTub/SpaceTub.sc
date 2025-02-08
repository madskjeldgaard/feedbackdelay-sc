SpaceTub : UGen {
    *ar { |in, maxdelaytime = 0.2, delaytime = 0.2, feedback = 0.5, cutoff=2500, resonance=0.75, filterType=0, limiterOn=1, limiterRelease=0.5, limiterHoldTime=0.1, useSoftClip=1|
        ^this.multiNew('audio', in, maxdelaytime, delaytime, feedback, cutoff, resonance, filterType, limiterOn, limiterRelease, limiterHoldTime, useSoftClip);
    }
}
