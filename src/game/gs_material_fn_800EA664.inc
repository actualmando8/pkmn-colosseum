    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    beq @L_800EA6C0
    bne @L_800EA690
    la r3, lbl_8047CC00(r2)
    li r4, 0x25d
    la r5, lbl_8047CC08(r2)
    bl fn_80196E10
    @L_800EA690:
    lwz r4, 0x14(r31)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800EA6AC
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800EA6AC
    li r3, 0x1
    @L_800EA6AC:
    cmpwi r3, 0x0
    bne @L_800EA6B8
    b @L_800EA6C0
    @L_800EA6B8:
    mr r3, r31
    bl fn_8019D9DC
    @L_800EA6C0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
