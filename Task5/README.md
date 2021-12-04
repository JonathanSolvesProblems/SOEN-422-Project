# SOEN422-F2021-CSLoader

Instructions and useful files for the CSLoader

## Prerequisites

- Install the [.NET SDK](https://dotnet.microsoft.com/download) for your platform.
- Ensure `dotnet` is in your PATH environment variable.
- Check `dotnet` version is above 5.0

  ```bash
  dotnet --version
  ```

Note: Running in a [docker container won't work](https://github.com/docker/for-win/issues/1018) because you need access to your computer's serial interface.

## Building/running the loader

1. Change [line 120](https://github.com/jonZlotnik/SOEN422-F2021-CSLoader/blob/main/Program.cs#L120) in Program.cs to match the port to which your Arduino Nano is connected.

2. From root of this repo:

```bash
dotnet run
```
