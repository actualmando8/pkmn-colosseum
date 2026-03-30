    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr. r28, r3
    beq @L_802078DC
    rlwinm r31, r4, 0, 16, 31
    li r29, 0x0
    b @L_802078D0
@L_802077F8:
    rlwinm r0, r29, 0, 16, 31
    mulli r0, r0, 0x154
    add. r30, r28, r0
    beq @L_802078CC
    mr r3, r30
    li r4, 0x0
    li r5, 0xcb
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r30
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    bl fn_80124A60
    mr r3, r30
    li r4, 0x0
    li r5, 0xcd
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x1
    bl fn_8011B950
    mr r3, r30
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    li r7, -0x1
    bl fn_801254B4
    mr r3, r30
    li r4, 0x0
    li r5, 0xcf
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r30
    li r4, 0x0
    li r5, 0xd0
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r30
    li r4, 0x0
    li r5, 0xd1
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r30
    li r4, 0x0
    li r5, 0xd2
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
@L_802078CC:
    addi r29, r29, 0x1
@L_802078D0:
    rlwinm r0, r29, 0, 16, 31
    cmplw r0, r31
    blt @L_802077F8
@L_802078DC:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
