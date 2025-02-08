VAStateVariableFilter : UGen {
    *ar { |in, cutoff=1500, resonance=0.5, filterType=0|
        ^this.multiNew('audio', in, cutoff, resonance, filterType);
    }
}
