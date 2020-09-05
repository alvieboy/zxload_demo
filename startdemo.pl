#!/usr/bin/perl

use POSIX ":sys_wait_h";
#use strict;

my @demos = (
             ['root','spectrum.png', 4],
             
             ['emul','loader.sna -load-immed intro.tap', 60, 4],

             ['img','teaser1.png', 4, 6],

             ['emul','manic.sna',30, 4],
             
             ['root','spectrum4.png', 4],

             ['img','teaser2.png', 6, 4],

             ['emul','backtoskool.sna',30, 4],
             
             ['root','spectrum.png', 4],

             ['img','teaser3.png', 6, 4],
             
             ['emul','thegreatescape.sna',20, 4],
             
             ['root','spectrum4.png', 4],

             ['img','teaser4.png', 6, 4],
             
             ['emul','chequeredflag.sna',20, 4],
             
             ['root','spectrum.png', 4],

             ['img','teaser2.png', 6, 4],

             ['emul','madmix.sna',20, 4],
             
             ['root','spectrum4.png', 4],

             ['img','teaser4.png', 6, 4],

             ['emul','rocknroller.sna',100, 4],
             
             ['root','spectrum.png', 4],
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
            my $kid = waitpid($pid, WNOHANG);
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

my $bgfile = 'spectrum.png';

system("/usr/bin/xli -dispgamma 1.0 -fit -fillscreen -onroot spectrum.png");

while (1) {
    foreach my $d (@demos) {
        my $dcmd = $d->[0];
        if ($dcmd eq 'emul') {
            runfor("./xspect -scale -1 ".$d->[1], $d->[2]);
            if (defined $d->[3]) {
                sleep $d->[3];
            }
        } elsif ($dcmd eq 'root') {
            my $file = $d->[1];
            system("/usr/bin/xli -dispgamma 1.0  -fit -fillscreen -onroot ${file}");
            $bgfile = $file;
            sleep $d->[2];
        }  elsif ($dcmd eq 'img') {
            my $file = $d->[1];
            runfor("/usr/bin/xli -dispgamma 1.0  -fit -fillscreen ${file}", $d->[2]);
            #system("/usr/bin/xli -dispgamma 1.0  -fit -fillscreen -onroot ${bgfile}");
            if (defined ($d->[3])) {
                sleep $d->[3];
            }
        } else {
            sleep 1;
        }
    }
}
#