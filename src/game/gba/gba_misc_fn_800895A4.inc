    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r30, r3
    mr r31, r4
    lbz r0, 0x0(r31)
    rlwinm r0, r0, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_800895D4
    li r0, 0x2
    b @L_800895D8
@L_800895D4:
    li r0, 0x1
@L_800895D8:
    rlwinm r4, r0, 0, 24, 31
    bl fn_8012A774
    li r3, 0x0
    li r4, 0x5
    bl fn_80135938
    mr r6, r3
    addi r3, r1, 0x8
    addi r4, r31, 0x4
    li r5, 0x7
    bl fn_800F9C04
    mr r3, r30
    addi r4, r1, 0x8
    bl fn_8012AA64
    lbz r4, 0xc(r31)
    mr r3, r30
    bl fn_8012AA44
    lwz r6, 0x10(r31)
    mr r3, r30
    rlwinm r0, r6, 0, 16, 23
    rlwinm r5, r6, 0, 8, 15
    rlwinm r4, r6, 24, 0, 7
    rlwinm r6, r6, 8, 24, 31
    rlwinm r0, r0, 8, 0, 23
    rlwinm r5, r5, 24, 8, 31
    or r0, r4, r0
    or r0, r5, r0
    or r4, r6, r0
    bl fn_8012AA54
    mr r29, r31
    li r28, 0x0
@L_80089650:
    mr r3, r30
    rlwinm r4, r28, 0, 16, 31
    bl fn_8012AC08
    addi r4, r29, 0x14
    mr r27, r3
    bl fn_8008BBDC
    mr r3, r27
    rlwinm r4, r28, 0, 16, 31
    bl fn_8011D494
    addi r29, r29, 0x64
    addi r28, r28, 0x1
    cmpwi r28, 0x6
    blt @L_80089650
    li r29, 0x0
@L_80089688:
    addi r0, r29, 0x26c
    mr r3, r29
    lbzx r4, r31, r0
    bl fn_80265EC4
    addi r29, r29, 0x1
    cmpwi r29, 0xb
    blt @L_80089688
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
