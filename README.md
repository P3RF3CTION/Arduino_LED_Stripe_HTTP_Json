# Arduino Adafruit LED Stripe based on HTTP Json response
Based on the Planetside 2 Census API this Arduino File is setting up your LED Stripe.
The current Configuration is based on my Planezside 2 Players and Accounts I have.
If a Player (P3RF3CTION) goes Online the Adafruit LED Stripe Changes the Colour to Red as the Faction for this Character is TR with the Colour Red.
If one of my other Accounts is going on the Colour is set to the Characters Faction Colour (Purple for VS or Blue for NC)
If you go Offline the LED Stripe turns off as well

## How it works ##

```mermaid
graph TD;
  Get Player Status from Planetside 2 API-->Parse the Player JSON;
  Parse the Player JSON-->TR_Account_Online;
  Parse the Player JSON-->NC_Account_Online;
  Parse the Player JSON-->VS_Account_Online;
  GetOutfitData-->New_per_Day;
```
