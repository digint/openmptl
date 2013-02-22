#!/usr/bin/perl

#
# creates sources which should fail at compile-time by succesively
# enabling all UNITTEST_STATIC_ASSERT.
#
# usage: create_fail_src.pl -o <output_dir> <srcs>...
#

use Getopt::Std;
use File::Spec::Functions qw(splitpath);
use strict;
use warnings FATAL => qw( all );

my @outfiles;

my %opts;
getopts('o:', \%opts);
my $outdir = $opts{o};

die unless(-d $outdir);

while (my $infile = shift @ARGV) {
  my (undef, $dir, $file) = splitpath($infile);
  my $fileext = $& if($file =~ s/\.\w+$//);
  my $outfile_prefix = $outdir . '/' . $file . '_';

  print STDERR "Reading file: $infile\n";

  my $text;

  open(INFILE, "<$infile") || die;
  {
    local $/;                   # slurp the file
    $text = <INFILE>;
  }
  close(INFILE);

  my $pre = '';
  my $count = 0;

  while ($text =~ /^\s*UNITTEST_STATIC_ASSERT[^\n]*\n/ms) {
    my $m = $&;
    $pre = $pre . $`;
    $text = $';
    my $mm = $m;
    $mm =~ s/UNITTEST_STATIC_ASSERT/UNITTEST_STATIC_ASSERT_ENABLED/;

    $count++;
    my $outfile = $outfile_prefix . $count . $fileext;

    print STDERR "Creating output file: $outfile\n";
    open(OUTFILE, ">$outfile") || die;
    print OUTFILE $pre . $mm .  $text;
    close(OUTFILE);
    push @outfiles, $outfile;

    $pre = $pre . $m;
  }
}


print join(' ', @outfiles);
