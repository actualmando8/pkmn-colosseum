    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl fn_801F025C
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    lwz r4, lbl_8047B610(r13)
    mr r31, r3
    lbz r7, 0x1(r4)
    cmplwi r7, 0x0
    beq @L_8021B67C
    li r4, 0x0
    li r5, 0x31
    li r6, 0x0
    bl fn_8011BBD8
    b @L_8021B6EC
@L_8021B67C:
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    rlwinm r0, r3, 30, 0, 1
    rlwinm r3, r4, 1, 31, 31
    subf r0, r3, r0
    rlwinm r0, r0, 2, 0, 31
    add r0, r0, r3
    rlwinm r3, r0, 0, 24, 31
    cmplwi r3, 0x2
    bge @L_8021B6B0
    addi r0, r3, 0x2
    rlwinm r0, r0, 0, 24, 31
    b @L_8021B6D4
@L_8021B6B0:
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    rlwinm r0, r3, 30, 0, 1
    rlwinm r3, r4, 1, 31, 31
    subf r0, r3, r0
    rlwinm r0, r0, 2, 0, 31
    add r3, r0, r3
    addi r0, r3, 0x2
    rlwinm r0, r0, 0, 24, 31
@L_8021B6D4:
    mr r3, r31
    rlwinm r7, r0, 0, 24, 31
    li r4, 0x0
    li r5, 0x31
    li r6, 0x0
    bl fn_8011BBD8
@L_8021B6EC:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x2
    stw r0, lbl_8047B610(r13)
    lwz r31, 0xc(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
