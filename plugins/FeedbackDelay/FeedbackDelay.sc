FeedbackDelay : UGen {
    *ar { |in, maxdelaytime = 0.2, delaytime = 0.2, feedback = 0.5|
        ^this.multiNew('audio', in, maxdelaytime, delaytime, feedback);
    }
}
