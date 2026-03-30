    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    li r3, 0x0
    bl fn_801F4718
    li r3, 0x0
    li r4, 0x1
    bl fn_801F3B24
    li r3, 0x0
    li r4, 0x0
    bl fn_8020BC94
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8020BC40
    b @L_8020BC80
@L_8020BC40:
    bl fn_801EF634
    rlwinm. r0, r3, 0, 16, 31
    beq @L_8020BC54
    li r3, 0x1
    b @L_8020BC80
@L_8020BC54:
    mr r3, r31
    bl fn_80211A00
    li r3, 0x0
    li r4, 0x1
    bl fn_8020BC94
    rlwinm r0, r3, 0, 24, 31
    li r4, 0x1
    cmplwi r0, 0x1
    beq @L_8020BC7C
    mr r4, r3
@L_8020BC7C:
    mr r3, r4
@L_8020BC80:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
