stwu r1, -0x40(r1)
mflr r0
stw r0, 0x44(r1)
stmw r25, 0x24(r1)
mr r26, r3
lhz r0, 0x0(r26)
mr r31, r4
mr r3, r31
li r29, 0x0
sth r0, 0x2(r26)
li r4, 0x0
bl fn_800F7A08
mr r27, r3
mr r3, r31
li r4, 0x0
bl fn_800F7A7C
extsb r0, r3
cmpwi r0, 0x0
bge @80068468
neg r0, r0
@80068468
cmpwi r0, 0x20
bgt @80068488
extsb r0, r27
cmpwi r0, 0x0
bge @80068480
neg r0, r0
@80068480
cmpwi r0, 0x20
ble @8006858C
@80068488
extsb r4, r3
extsb r0, r27
lis r3, 0x4330
lfd f2, lbl_8047BFF0@sda21(r0)
xoris r4, r4, 0x8000
xoris r0, r0, 0x8000
stw r4, 0xc(r1)
stw r3, 0x8(r1)
lfd f0, 0x8(r1)
stw r0, 0x14(r1)
fsub f1, f0, f2
stw r3, 0x10(r1)
lfd f0, 0x10(r1)
fsub f2, f0, f2
bl fn_800CE2D8
frsp f2, f1
lfs f0, lbl_8047BFE8@sda21(r0)
fcmpo cr0, f2, f0
ble @800684DC
fmr f1, f2
b @800684E0
@800684DC
fneg f1, f2
@800684E0
lfs f0, lbl_8047BFF8@sda21(r0)
fcmpo cr0, f1, f0
bge @800684F8
ori r0, r29, 0x2
clrlwi r29, r0, 16
b @80068524
@800684F8
lfs f0, lbl_8047BFE8@sda21(r0)
fcmpo cr0, f2, f0
ble @8006850C
fmr f1, f2
b @80068510
@8006850C
fneg f1, f2
@80068510
lfs f0, lbl_8047BFFC@sda21(r0)
fcmpo cr0, f1, f0
ble @80068524
ori r0, r29, 0x1
clrlwi r29, r0, 16
@80068524
lfs f0, lbl_8047BFE8@sda21(r0)
lfs f1, lbl_8047C000@sda21(r0)
fcmpo cr0, f2, f0
ble @8006853C
fmr f0, f2
b @80068540
@8006853C
fneg f0, f2
@80068540
fcmpo cr0, f1, f0
bge @8006858C
lfs f0, lbl_8047BFE8@sda21(r0)
fcmpo cr0, f2, f0
ble @8006855C
fmr f1, f2
b @80068560
@8006855C
fneg f1, f2
@80068560
lfs f0, lbl_8047C004@sda21(r0)
fcmpo cr0, f1, f0
bge @8006858C
lfs f0, lbl_8047BFE8@sda21(r0)
fcmpo cr0, f2, f0
bge @80068584
ori r0, r29, 0x4
clrlwi r29, r0, 16
b @8006858C
@80068584
ori r0, r29, 0x8
clrlwi r29, r0, 16
@8006858C
mr r3, r31
bl fn_800F7BC4
rlwinm r0, r3, 0, 28, 28
cmplwi r0, 0x0
beq @800685A8
ori r0, r29, 0x1
clrlwi r29, r0, 16
@800685A8
rlwinm r0, r3, 0, 29, 29
cmplwi r0, 0x0
beq @800685BC
ori r0, r29, 0x2
clrlwi r29, r0, 16
@800685BC
clrlwi r0, r3, 31
cmplwi r0, 0x0
beq @800685D0
ori r0, r29, 0x4
clrlwi r29, r0, 16
@800685D0
rlwinm r0, r3, 0, 30, 30
cmplwi r0, 0x0
beq @800685E4
ori r0, r29, 0x8
clrlwi r29, r0, 16
@800685E4
rlwinm r0, r3, 0, 23, 23
cmplwi r0, 0x0
beq @800685F8
ori r0, r29, 0x10
clrlwi r29, r0, 16
@800685F8
rlwinm r0, r3, 0, 22, 22
cmplwi r0, 0x0
beq @8006860C
ori r0, r29, 0x20
clrlwi r29, r0, 16
@8006860C
rlwinm r0, r3, 0, 21, 21
cmplwi r0, 0x0
beq @80068620
ori r0, r29, 0x40
clrlwi r29, r0, 16
@80068620
rlwinm r0, r3, 0, 20, 20
cmplwi r0, 0x0
beq @80068634
ori r0, r29, 0x80
clrlwi r29, r0, 16
@80068634
rlwinm r0, r3, 0, 27, 27
cmplwi r0, 0x0
beq @80068648
ori r0, r29, 0x100
clrlwi r29, r0, 16
@80068648
rlwinm r0, r3, 0, 25, 25
cmplwi r0, 0x0
beq @8006865C
ori r0, r29, 0x200
clrlwi r29, r0, 16
@8006865C
rlwinm r0, r3, 0, 26, 26
cmplwi r0, 0x0
beq @80068670
ori r0, r29, 0x400
clrlwi r29, r0, 16
@80068670
rlwinm r0, r3, 0, 19, 19
cmplwi r0, 0x0
beq @80068684
ori r0, r29, 0x800
clrlwi r29, r0, 16
@80068684
lhz r0, 0x2(r26)
clrlwi r31, r29, 16
li r27, 0x0
li r30, 0x0
xori r0, r0, 0xffff
and r0, r0, r31
clrlwi r28, r0, 16
@800686A0
li r0, 0x1
slw r0, r0, r30
clrlwi r25, r0, 16
and r0, r28, r25
cmpwi r0, 0x0
beq @800686CC
addi r0, r30, 0xa
li r3, 0xf
stbx r3, r26, r0
or r27, r27, r25
b @8006870C
@800686CC
and r0, r31, r25
cmpwi r0, 0x0
beq @8006870C
bl fn_800D3088
addi r4, r30, 0xa
lbzx r0, r26, r4
subf r0, r3, r0
extsb r0, r0
stbx r0, r26, r4
lbzx r0, r26, r4
extsb r0, r0
cmpwi r0, 0x0
bgt @8006870C
li r0, 0x5
or r27, r27, r25
stbx r0, r26, r4
@8006870C
addi r30, r30, 0x1
cmpwi r30, 0x10
blt @800686A0
sth r29, 0x0(r26)
sth r28, 0x4(r26)
sth r27, 0x6(r26)
lmw r25, 0x24(r1)
lwz r0, 0x44(r1)
mtlr r0
addi r1, r1, 0x40
blr
