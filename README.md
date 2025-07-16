# Skeleton setup for an iOs audio app

This repo is an app skeleton that uses [miniaudio](https://miniaud.io) and [Swift Bundler](https://swiftbundler.dev/) to build audio apps for iOs.

It assumes a C backend that is then used in the SwiftUi frontend.

The main goal is to provide a solid starting ground to create audio apps for iPhone without needing to touch XCode.

## Build and Run

This project uses [Just](https://just.systems) as a task runner and the justfile should be self expanatory. 

Building and running is simply a matter of calling :

```
just
```

to run a debug build on your machine, 


```
just simulate
```


to run on an iPhone simulator or 


```
just device <your-iphone>
``` 

to run on your actual (plugged in via Usb) iPhone.




