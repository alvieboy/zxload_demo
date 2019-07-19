#!/bin/perl

use POSIX ":sys_wait_h";

my @demos = (
             ['loader.sna -load-immed intro.tap', 60],
             ['manic.sna',110],
             ['backtoskool.sna',110],
             ['thegreatescape.sna',60],
             ['chequeredflag.sna',60],
             ['madmix.sna',60],
             ['rocknroller.sna',100],
            );


sub runfor
{
    my ($app, $timeout) = @_;
    
    my $pid = fork();
    return -1 if ($pid<0);
    if ($pid) {
        sleep $timeout;
        print("Pid: $pid\n");
        kill(SIGTERM, $pid);
        print("Killed $pid\n");
        
        sleep 1;
        while (1) {
            $kid = waitpid($pid, WNOHANG);
            if ($kid!=$pid) {
                kill(SIGKILL,$pid);
            }
            return 0;
        }
    } else {
        exit exec($app);
    }
}


#open(STDOUT,'>/dev/null');
#open(STDERR,'>/dev/null');


$ENV{'DISPLAY'}=":0.0" unless (exists $ENV{'DISPLAY'});

system("/usr/bin/xli dispgamma 1.0 -fit -fillscreen -onroot spectrum.png");

while (1) {
    foreach my $d (@demos) {

        runfor("./xspect -scale -1 ".$d->[0], $d->[1]);
        sleep 4;
    }
}
#