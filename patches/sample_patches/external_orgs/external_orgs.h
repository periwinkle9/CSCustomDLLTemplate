/* Allows <CMUxxxx to load an org from data / Org / xxxx.org.
   Note that xxxx still needs to be a number (if using OOB TSC values, the file name needs to be the actual number).
   Examples: <CMU0003 -> data/Org/0003.org, <CMUabcd -> data/Org/54562.org
   If the file does not exist, then it loads the vanilla org embedded inside the exe.
   (If that doesn't exist either, then it loads track 0.)
*/

#pragma once

namespace external_orgs
{
bool applyPatch();
}
