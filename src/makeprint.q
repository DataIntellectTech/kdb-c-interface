make:`:./libqtoc 2:(`makeq;1)
print:`:./libqtoc 2:(`printq;1)

-1"##";
-1"# makeprint.q";
-1"# ";
-1"# Example q script that loads in two functions from the libqtoc shared object";
-1"# and makes them available as 'make' and 'print'.";
-1"#";
-1"# AquaQ Analytics";
-1"# kdb+ consultancy, training and support";
-1"#";
-1"# For questions, comments, requests or bug reports, please contact us";
-1"# w: www.aquaq.co.uk";
-1"# e: support@aquaq.co.uk";
-1"#";
-1"# examples:";
-1"# \tprint[1b] to print an atom";
-1"# \tprint[101b] to print a list";
-1"# \tprint[`a`b`c!1 2 3] to print a dictionary]";
-1"# \tprint[([] a:til 5; b:reverse `char$65+til 5)] to print a table";
-1"#";
-1"# \tmake[`b] to create a atom";
-1"# \tmake[`B] to create a list";
-1"# \tmake[`dictionary] to create a dictionary";
-1"# \tmake[`table] to create a table\n\n";
