## XML Event Processing Command Line Application

C application that outputs xml events into an easy to read format in your native systems terminal.

### Sample Xml Input File

``` xml

<calendar>
  <event>
    <description>First commit</description>
    <timezone>GMT-2</timezone>
    <location>Engineering Wing</location>
    <day>20</day>
    <month>02</month>
    <year>2022</year>
    <dweek>Friday</dweek>
    <start>11:00</start>
    <end>13:00</end>
  </event>
</calendar>

```

### Sample command line input
```
./process_cal --start=2022/1/1 --end=2022/2/21 --file=sample_input.xml
```
The output that would be produced is:
```

February 20, 2022 (Sunday)
--------------------------
11:00 AM to 01:00 PM: First commit {{Engineering Wing}} | GMT-2

```


