    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r31, r4
    mr r27, r3
    mr r28, r5
    mr r29, r6
    mr r3, r31
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm. r4, r3, 0, 16, 31
    bne @L_8025E0F4
    li r3, 0x0
    b @L_8025E1A8
@L_8025E0F4:
    li r3, 0x0
    li r5, 0x66
    li r6, 0x0
    bl fn_8012640C
    rlwinm. r30, r3, 0, 16, 31
    bne @L_8025E114
    li r3, 0x0
    b @L_8025E1A8
@L_8025E114:
    mr r3, r31
    bl fn_8011F5B0
    mr r0, r3
    mr r3, r31
    mr r31, r0
    li r4, 0x0
    li r5, 0xc1
    li r6, 0x0
    bl fn_8012640C
    neg r0, r3
    mr r4, r31
    or r0, r0, r3
    mr r3, r30
    rlwinm r5, r0, 1, 31, 31
    bl fn_801DE190
    cmplwi r3, 0x0
    stw r3, 0x0(r27)
    bne @L_8025E164
    li r3, 0x0
    b @L_8025E1A8
@L_8025E164:
    mr r31, r28
    li r30, 0x0
    b @L_8025E19C
@L_8025E170:
    lwz r3, 0x0(r27)
    li r6, 0x0
    lhz r4, 0x0(r31)
    lhz r5, 0x2(r31)
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8025E194
    li r3, 0x0
    b @L_8025E1A8
@L_8025E194:
    addi r31, r31, 0x4
    addi r30, r30, 0x1
@L_8025E19C:
    cmpw r30, r29
    blt @L_8025E170
    li r3, 0x1
@L_8025E1A8:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
