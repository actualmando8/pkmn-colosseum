    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    li r31, 0x0
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    sth r31, -21576(r13)
    b @L_801D0BA4
@L_801D0B20:
    li r30, 0x1
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    rlwinm r4, r31, 0, 16, 31
    bl fn_8012AC08
    mr r29, r3
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801D0B50
    li r30, 0x0
    b @L_801D0B8C
@L_801D0B50:
    mr r3, r29
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r29
    rlwinm r4, r0, 0, 16, 31
    li r5, 0x1
    li r6, 0x0
    bl fn_8012640C
    mr r3, r29
    bl fn_8011F550
    mr r3, r29
    bl fn_8012086C
@L_801D0B8C:
    rlwinm. r0, r30, 0, 24, 31
    beq @L_801D0BA0
    lhz r3, -21576(r13)
    addi r0, r3, 0x1
    sth r0, -21576(r13)
@L_801D0BA0:
    addi r31, r31, 0x1
@L_801D0BA4:
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x6
    blt @L_801D0B20
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lhz r3, -21576(r13)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
