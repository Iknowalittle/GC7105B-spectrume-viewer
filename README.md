# GC7105B-spectrum-viewer<br>

What does it do?<br>
  a simple tool that allow you to get GC7105B spectrum measurement data remotely<br>
  
How to use?<br>
  Hardware preparation: <br>
  1) you need config your GenComm GC7105B with fixed IP address, and connect to your local area network.<br>
  2) Select GC7105B working mode to Spectrume analyzer(current version only support this mode).<br>
  3) Set the frequency parameters by hand.(center frequency and span frequency).<br>
  
  Software:<br>
    By default, please install gnuplot first.  so you can get an image when you fetch data from GC7105B
    I just wrote it under Ubuntu, you can transfer it to any environment which you like.<br>
    Download the sample code, then enter the directory to compile it with command line.<br>
    For example: <br>
    g++ ./testgc7105.cpp ./gc7105b.cpp -ogc<br>
    Then you got an executable named with gc, <br>
    type ./gc <enter>you will get result output like this:<br>
      recv msg from client: BP<br>
      try to capture data<br>
      start  freq: 935000000Hz<br>
      stop   freq: 960000000Hz<br>
      center freq: 947500000Hz<br>
      span   freq: 25000000Hz<br>
      Peak Power: -69.82 at 959.90Mhz<br>
  
      Press ENTER to continue...<br>
  
![spec plot](https://user-images.githubusercontent.com/118705092/203482865-1693bded-622d-473b-8262-587cf7a7b0d4.png)

![peak power](https://user-images.githubusercontent.com/118705092/203482877-279d652a-b4aa-48b6-b02b-08b97e540ddc.png)
