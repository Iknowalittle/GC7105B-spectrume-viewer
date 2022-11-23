# GC7105B-spectrume-viewer

What does it do?
  a simple tool that allow you to get GC7105B spectrum measurement data remotely
  
How to use?
  Hardware preparation: 
  1) you need config your GenComm GC7105B with fixed IP address, and connect to your local area network.
  2) Select GC7105B working mode to Spectrume analyzer(current version only support this mode).
  3) Set the frequency parameters by hand.(center frequency and span frequency).
  
  Software:
    By default, please install gnuplot first.  so you can get an image when you fetch data from GC7105B
    I just wrote it under Ubuntu, you can transfer it to any environment which you like.
    Download the sample code, then enter the directory to compile it with command line.
    For example: 
    g++ ./testgc7105.cpp ./gc7105b.cpp -ogc
    Then you got an executable named with gc, 
    type ./gc <enter>you will get result output like this:
      recv msg from client: BP
      try to capture data
      start  freq: 935000000Hz
      stop   freq: 960000000Hz
      center freq: 947500000Hz
      span   freq: 25000000Hz
      Peak Power: -69.82 at 959.90Mhz
![spec plot](https://user-images.githubusercontent.com/118705092/203482865-1693bded-622d-473b-8262-587cf7a7b0d4.png)

      Press ENTER to continue...
    ![peak power](https://user-images.githubusercontent.com/118705092/203482877-279d652a-b4aa-48b6-b02b-08b97e540ddc.png)
