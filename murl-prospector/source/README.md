# Recovered MURL Robot Passport Source

These seven files preserve the source modules recovered from the Library artifact `MURL Robot Passport Cutebot Pro v0.1.0.ubp`.

They are stored separately because the GitHub connector is text-oriented. Concatenating the files in numeric order reconstructs the original MicroBlocks project source module order:

1. `00_main.ubp.txt`
2. `01_MURL_Core.ubp.txt`
3. `02_MURL_Safety_Service.ubp.txt`
4. `03_MURL_Evidence_Service.ubp.txt`
5. `04_MURL_Motion_Service.ubp.txt`
6. `05_MURL_Cutebot_Adapter.ubp.txt`
7. `06_MURL_Cutebot_Profile.ubp.txt`

## Validation boundary

The recovered profile identifies itself as `COMMISSIONING-UNVERIFIED`. It intentionally withholds measured distance and turn capability because that release had not established a public traceable encoder scale. Candidate native velocity, front-range, and line services can be enabled for physical commissioning. Preserve that distinction when reusing or describing this source.

The MURL modules identify their license as MPL-2.0 and describe MURL as an independent Robot EDU project, not an official MicroBlocks specification.
