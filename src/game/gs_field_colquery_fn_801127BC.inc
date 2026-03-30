    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r0, 0x1
    stb r0, lbl_80478DD0(r13)
    bl fn_80115BD8
    bl fn_80115AC8
    rlwinm r0, r3, 0, 24, 31
    cmpwi r0, 0x5
    bge @L_801127EC
    cmpwi r0, 0x1
    b @L_80112800
@L_801127EC:
    cmpwi r0, 0x7
    bge @L_80112800
    li r3, 0x1
    bl fn_800D3074
    b @L_80112808
@L_80112800:
    li r3, 0x2
    bl fn_800D3074
@L_80112808:
    lis r3, lbl_80408378@ha
    addi r3, r3, lbl_80408378@l
    lwz r0, 0x28(r3)
    cmplwi r0, 0x0
    beq @L_80112834
    bl fn_800FF560
    lis r5, -32751
    mr r4, r3
    addi r5, r5, 10308
    li r3, 0x0
    bl fn_800FF2A0
@L_80112834:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

