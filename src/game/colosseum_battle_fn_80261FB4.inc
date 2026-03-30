    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r29, r4
    mr r30, r3
    li r3, 0x2
    mr r4, r30
    mr r5, r29
    bl fn_801F02AC
    cmplwi r3, 0x0
    bne @L_80261FEC
    li r31, 0x0
    b @L_8026204C
@L_80261FEC:
    li r4, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_801F76B8
    rlwinm r31, r3, 0, 16, 31
    mr r3, r30
    mr r4, r29
    bl fn_801F0134
    rlwinm. r0, r3, 0, 16, 31
    bne @L_8026201C
    li r31, 0x0
    b @L_8026204C
@L_8026201C:
    bl fn_801F0234
    bl fn_801F0204
    cmpwi r3, 0x0
    bge @L_80262034
    li r31, 0x0
    b @L_8026204C
@L_80262034:
    mr r4, r31
    rlwinm r6, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x3
    bl fn_801F76B8
    mr r31, r3
@L_8026204C:
    mr r3, r31
    bl fn_80102620
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8026206C
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    bl fn_80102568
@L_8026206C:
    lmw r29, 0x14(r1)
    li r3, 0x1
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
